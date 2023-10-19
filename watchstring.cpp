#include "pin.H"
#include <iostream>
#include <cstring>
#include <fstream> 

// Launch it with ./pin -pid 43242 -t watchstring.so -s "UniqueString" -f "/path/to/logfile.txt"
// https://www.youtube.com/watch?v=A3iRjGdyoLo
// Download library https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-dynamic-binary-instrumentation-tool.html

std::ofstream logFile;

KNOB<std::string> TargetString(KNOB_MODE_WRITEONCE, "pintool", "s", "", "target string to detect");
KNOB<std::string> LogFilePath(KNOB_MODE_WRITEONCE, "pintool", "f", "", "path to the log file");

VOID WriteMem(VOID * ip, VOID * addr, UINT32 size) {
    const char* target = TargetString.Value().c_str();
    size_t target_length = strlen(target);
    
    for (UINT32 i = 0; i <= size - target_length; i++) {
        if (strncmp((char*)addr + i, target, target_length) == 0) {
            logFile << "Memory write containing '" << target << "' detected at IP: " << ip << std::endl;
            break;
        }
    }
}

VOID ReadMem(VOID * ip, VOID * addr, UINT32 size) {
    const char* target = TargetString.Value().c_str();
    size_t target_length = strlen(target);
    
    for (UINT32 i = 0; i <= size - target_length; i++) {
        if (strncmp((char*)addr + i, target, target_length) == 0) {
            logFile << "Memory read containing '" << target << "' detected at IP: " << ip << std::endl;
            break;
        }
    }
}

VOID Instruction(INS ins, VOID *v) {
    if (INS_IsMemoryWrite(ins)) {
        INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR)WriteMem,
            IARG_INST_PTR,
            IARG_MEMORYWRITE_EA,
            IARG_MEMORYWRITE_SIZE,
            IARG_END);
    }
    
    if (INS_IsMemoryRead(ins)) {
        INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR)ReadMem,
            IARG_INST_PTR,
            IARG_MEMORYREAD_EA,
            IARG_MEMORYREAD_SIZE,
            IARG_END);
    }
}

int main(int argc, char *argv[]) {
    if (PIN_Init(argc, argv)) {
        std::cerr << "Initialization error" << std::endl;
        return -1;
    }

    if (TargetString.Value().empty()) {
        std::cerr << "You must specify the target string with the -s option." << std::endl;
        return -1;
    }

    logFile.open(LogFilePath.Value().c_str(), std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open the log file: " << LogFilePath.Value() << std::endl;
        return -1;
    }

    INS_AddInstrumentFunction(Instruction, 0);
    
    PIN_StartProgram();
    
    logFile.close();

    return 0;
}
