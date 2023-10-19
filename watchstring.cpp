#include "pin.H"
#include <iostream>
#include <cstring>
#include <fstream> 

// Launch it with ./pin -pid 43242 -t watchstring.so -s "UniqueString" -f "/path/to/logfile.txt"
// https://www.youtube.com/watch?v=A3iRjGdyoLo
// Download library https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-dynamic-binary-instrumentation-tool.html

// Global ofstream object for writing to the file
std::ofstream logFile;

KNOB<std::string> TargetString(KNOB_MODE_WRITEONCE, "pintool", "s", "", "target string to detect");
KNOB<std::string> LogFilePath(KNOB_MODE_WRITEONCE, "pintool", "f", "", "path to the log file");

VOID WriteMem(VOID * ip, VOID * addr, UINT32 size) {
    const char* target = TargetString.Value().c_str();
    size_t target_length = strlen(target);
    // logFile << "Memory write detected at " << ip << std::endl; // Add to debug
    
    // Loop through the memory write to see if it contains the target string
    for (UINT32 i = 0; i <= size - target_length; i++) {
        if (strncmp((char*)addr + i, target, target_length) == 0) {
            logFile << "Memory write containing '" << target << "' detected at IP: " << ip << std::endl;
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
}

int main(int argc, char *argv[]) {
    // Initialize pin
    if (PIN_Init(argc, argv)) {
        std::cerr << "Initialization error" << std::endl;
        return -1;
    }

    if (TargetString.Value().empty()) {
        std::cerr << "You must specify the target string with the -s option." << std::endl;
        return -1;
    }

    // Open the log file
    logFile.open(LogFilePath.Value().c_str(), std::ios::out | std::ios::app); // Open in append mode
    if (!logFile.is_open()) {
        std::cerr << "Failed to open the log file: " << LogFilePath.Value() << std::endl;
        return -1;
    }

    INS_AddInstrumentFunction(Instruction, 0);
    
    // Start the program
    PIN_StartProgram();
    
    // Close the log file (though this point might not be reached if the program doesn't naturally terminate)
    logFile.close();

    return 0;
}
