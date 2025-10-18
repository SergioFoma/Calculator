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
    DRAW        = 10,
    MOD         = 11,
    POPR        = 42,
    PUSHR       = 33,
    JB          = 50,
    JAE         = 53,
    CALL        = 60,
    RET         = 61,
    PUSHM       = 70,
    POPM        = 71
};

enum calculatorErrors {
    SUCCESSFUL                   = 0,
    DIV_BY_NULL                  = 1,
    FEW_ELEMENTS                 = 2,
    THE_ROOT_OF_NEGATIVE_NUMBERS = 3,
    ERROR_FROM_SPU               = 4
};

void SPULoop( stack_t *stk );

calculatorErrors calculationFromProcessor( Processor *SPU, const char* byteFile );

void doPush( Processor* SPU );

calculatorErrors doMathOperation( Processor* SPU, int( *mathFunction )( int first, int last) );

int sumNumbers( int first, int last );

int subNumbers( int first, int last );

int mulNumbers( int first, int last );

int divNumbers( int first, int last );

int modNumbers( int first, int last );

void doOut( Processor* SPU );

calculatorErrors doSqrt( Processor* SPU );

void doIn( Processor* SPU );

calculatorErrors doPopr( Processor* SPU );

void doPushr( Processor* SPU );

calculatorErrors doJB( Processor* SPU );

calculatorErrors doJAE( Processor* SPU );

void doCall( Processor* SPU );

calculatorErrors doRet( Processor* SPU );

void doPushm( Processor* SPU );

calculatorErrors doPopm( Processor* SPU );

#define checkCalculatorStatus( typeOfErr )          \
    if( typeOfErr != SUCCESSFUL ){                  \
        return typeOfErr;                           \
    }
#endif
