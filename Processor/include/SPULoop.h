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
    PUSHR       = 33
};

void SPULoop( stack_t *stk );

void calculationFromProcessor( Processor *SPU, const char* byteFile );

#define DO_PUSH                                                           \
    stackPush( &(SPU->stk), (SPU->code).command[ ++index ] );             \
    stackPrint( &(SPU->stk) );

// TODO use functions instead of macros
#define DO_MUL                                                      \
    last = stackPop( &(SPU->stk) );                                 \
    first = stackPop( &(SPU->stk) );                                \
    stackPush( &(SPU->stk), first * last );                         \
    stackPrint( &(SPU->stk) ); // TODO make single macro for sub, div, add, mul

#define DO_SUB                                                      \
    last = stackPop( &(SPU->stk) );                                 \
    first = stackPop( &(SPU->stk) );                                \
    stackPush( &(SPU->stk), first - last );                         \
    stackPrint( &(SPU->stk) );

#define DO_OUT                                                      \
    printf("%d\n", stackPop( &(SPU->stk ) ) );                      \
    stackPrint( &(SPU->stk) );

#define DO_ADD                                                      \
    last = stackPop( &(SPU->stk) );                                 \
    first = stackPop( &(SPU->stk) );                                \
    stackPush( &(SPU->stk) , first + last );                        \
    stackPrint( &(SPU->stk) );

#define DO_DIV                                                      \
    last = stackPop( &(SPU->stk) );                                 \
    first = stackPop( &(SPU->stk) );                                \
    if ( last != 0 ){                                               \
        stackPush( &(SPU->stk), first / last );                     \
    }                                                               \
    stackPrint( &(SPU->stk) );

#define DO_SQRT                                                     \
    last = stackPop( &(SPU->stk) );                                 \
    if( last >= 0 ){                                                \
        printf("%d\n", (int)sqrt( last ) );                         \
    }                                                               \
    stackPrint( &(SPU->stk) );

#define DO_IN                                                       \
    colorPrintf( NOMODE, BLUE, "Enter number from keyboard: " );    \
    scanf("%d", &first );                                           \
    stackPush( &(SPU->stk), first );                                \
    stackPrint( &(SPU->stk) );

#define DO_POPR                                                     \
    last = stackPop( &(SPU->stk) );                                 \
    indexOfRegister = (SPU->code).command[ ++index ];               \
    (SPU->regs)[ indexOfRegister ] = last;                          \
    (SPU->indexForRegister) = (regsIndex)indexOfRegister;           \
    stackPrint( &(SPU->stk) );                                      \
    regsPrint( SPU );

#define DO_PUSHR                                                    \
    indexOfRegister = (SPU->code).command[ ++index ];               \
    SPU->indexForRegister = (regsIndex)( indexOfRegister );         \
    stackPush( &(SPU->stk), (SPU->regs)[ indexOfRegister ] );       \
    stackPrint( &(SPU->stk) );                                      \
    regsPrint( SPU );

#define DO_SOLVE_EQUATION                                           \
    Coefficients coefficients = {1, -2, -8};                        \
    SolveResult answer = { NAN, NAN, zeroRoot};                     \
    solveEquation( coefficients, &answer );                         \
    printResult( coefficients, answer );

#endif
