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
    JAE         = 53,
    CALL        = 60,
    RET         = 61,
    PUSHM       = 70,
    POPM        = 71
};

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

void doJAE( Processor* SPU );

void doCall( Processor* SPU );

void doRet( Processor* SPU );

void doPushm( Processor* SPU );

void doPopm( Processor* SPU );

#endif
