#ifndef H_SOFTPROCESSOR
#define H_SOFTPROCESSOR

#include <stdio.h>

#include "onegin.h"
#include "stack.h"
#include "checkError.h"

enum regsIndex {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3
};

enum processorError {
    CORRECT_SPU    = 0,
    STACK_ERROR    = 1,
    CODE_NULL_PTR  = 2,
    REGS_NULL_PTR  = 3,
    SPU_DESTROY    = 4,
};

struct Code {
    int* command;
    size_t sizeOfCommands;
};

struct Processor{
    stack_t stk;
    Code code;
    size_t instructionPointer;
    int regs[10];
    regsIndex indexForRegister;
    processorError spuErr;
};

void softProcessor( const char* nameOfByteFile, Processor* SPU );

void getArrayWithCommand( strInformation stringFromFile, Processor* SPU );

void processorInit( Processor* SPU );

void processorDestroy( Processor* SPU );

processorError processorVerify( Processor* SPU );


#endif
