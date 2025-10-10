#ifndef H_SPULOOP
#define H_SPULOOP

#include "stack.h"
#include "softProcessor.h"

enum doCommand {
    HLT         = 0,
    PUSH        = 1,
    MUL         = 2,
    SUB         = 3,
    OUT         = 4,
    ADD         = 5,
    DIV         = 6,
    SQRT        = 7,
    IN          = 9,
    SOLVER      = 10,
    POPR        = 42,
    PUSHR       = 33,
    JB          = 50,
    JA          = 51
};

/*struct spuCommand {
    doCommand typeOfCommand;
    void( *function )( Processor* SPU );
};*/

void SPULoop( stack_t *stk );

void calculationFromProcessor( Processor *SPU, const char* byteFile );

void doPush( Processor* SPU );

void doMathOperation( Processor* SPU, int typeOfOperation);

void doOut( Processor* SPU );

void doSqrt( Processor* SPU );

void doIn( Processor* SPU );

void doPopr( Processor* SPU );

void doPushr( Processor* SPU );

void doSolveEquation( Processor* SPU );

void doJB( Processor* SPU );

void doJA( Processor* SPU );

#endif
