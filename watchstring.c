#include "pin.H"
#include <iostream>
#include <cstring>

// Launch with ./pin -pid 5532 -t watchstring.so -s "UniqueString"

KNOB<string> TargetString(KNOB_MODE_WRITEONCE, "pintool", "s", "", "target string to detect");

VOID WriteMem(VOID * ip, VOID * addr, UINT32 size) {
    const char* target = TargetString.Value().c_str();
    size_t target_length = strlen(target);
    
    // Loop through the memory write to see if it contains the target string
    for (UINT32 i = 0; i <= size - target_length; i++) {
        if (strncmp((char*)addr + i, target, target_length) == 0) {
            printf("Memory write containing '%s' detected at IP: %p\n", target, ip);
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

    INS_AddInstrumentFunction(Instruction, 0);
    
    // Start the program
    PIN_StartProgram();
    
    return 0;
}
