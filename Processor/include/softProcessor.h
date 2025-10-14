#ifndef H_SOFTPROCESSOR
#define H_SOFTPROCESSOR

#include <stdio.h>

#include "onegin.h"
#include "stack.h"

const size_t sizeOfRegs = 10;

enum regsIndex {
    RAX             = 0,
    RBX             = 1,
    RCX             = 2,
    RDX             = 3,
    NOT_IDENTIFIED  = 9
};

enum processorError {
    CORRECT_SPU    = 0,
    STACK_ERROR    = 1,
    CODE_NULL_PTR  = 2,
    REGS_NULL_PTR  = 3,
    SPU_DESTROY    = 4,
    SPU_NULL_PTR   = 5
};

struct Code {
    int* command;
    size_t sizeOfCommands;
};

struct Processor{
    stack_t stk;
    Code code;
    size_t instructionPointer;
    int regs[ sizeOfRegs ];
    regsIndex indexForRegister;
    processorError spuErr;
};

void softProcessor( const char* nameOfByteFile, Processor* SPU );

void getArrayWithCommand( bufferInformation bufferFromFile, Processor* SPU );

void processorInit( Processor* SPU );

void processorDestroy( Processor* SPU );

void regsPrint( Processor* SPU );

#endif
