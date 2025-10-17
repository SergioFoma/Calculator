#include <stdio.h>
#include <string.h>
#include <cmath>

#include "SPULoop.h"
#include "softProcessor.h"
#include "paint.h"
#include "checkError.h"

void calculationFromProcessor( Processor *SPU, const char* byteFile ){
    SPU_OK( SPU );

    softProcessor( byteFile, SPU );
    int DO = 1;

    while( (SPU->instructionPointer) < (SPU->code).sizeOfCommands && DO ){
        switch( (SPU->code).command[ SPU->instructionPointer ] ){
            case HLT:
                DO = 0;
                break;
            case PUSH:
                doPush( SPU );
                break;
            case MUL:
                doMathOperation( SPU, mulNumbers );
                break;
            case SUB:
                doMathOperation( SPU, subNumbers );
                break;
            case ADD:
                doMathOperation( SPU, sumNumbers );
                break;
            case DIV:
                doMathOperation( SPU, divNumbers );
                break;
            case OUT:
                doOut( SPU );
                break;
            case SQRT:
                doSqrt( SPU );
                break;
            case IN:
                doIn( SPU );
                break;
            case POPR:
                doPopr( SPU );
                break;
            case PUSHR:
                doPushr( SPU );
                break;
            case JB:
                doJB( SPU );
                break;
            case JAE:
                doJAE( SPU );
                break;
            case CALL:
                doCall( SPU );
                break;
            case RET:
                doRet( SPU );
                break;
            case PUSHM:
                doPushm( SPU );
                break;
            case POPM:
                doPopm( SPU );
                break;
            case DRAW:
                ramPrint( SPU );
                break;
            default:
                break;
        }
        ++(SPU->instructionPointer);
    }
}

void doPush( Processor* SPU ){
    stackPush( &(SPU->stk), (SPU->code).command[ ++(SPU->instructionPointer) ] );
    stackPrint( &(SPU->stk) );
}
int sumNumbers( int first, int last ){
    return first + last;
}
int mulNumbers( int first, int last ){
    return first * last;
}
int subNumbers( int first, int last ){
    return first - last;
}
int divNumbers( int first, int last ){
    if( last != 0 ){
        return first / last;
    }
    colorPrintf( NOMODE, RED, "\n\nDivision by zero\n\n" );
    return (int)0;
}
void doMathOperation( Processor* SPU, int( *mathFunction )( int first, int last)){
    int last = stackPop( &(SPU->stk) );
    int first = stackPop( &(SPU->stk) );
    stackPush( &(SPU->stk), mathFunction( first, last ) );
    stackPrint( &(SPU->stk) );
}
void doOut( Processor* SPU ){
    printf("%d\n", stackPop( &(SPU->stk ) ) );
    stackPrint( &(SPU->stk) );
}
void doSqrt( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    if( last >= 0 ){
        printf("%d\n", (int)sqrt( last ) );
    }
    stackPrint( &(SPU->stk) );
}
void doIn( Processor* SPU ){
    int first = 0;
    colorPrintf( NOMODE, BLUE, "Enter number from keyboard: " );
    scanf("%d", &first );
    stackPush( &(SPU->stk), first );
    stackPrint( &(SPU->stk) );
}
void doPopr( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    int indexOfRegister = (SPU->code).command[ ++(SPU->instructionPointer) ];
    (SPU->regs)[ indexOfRegister ] = last;
    (SPU->indexForRegister) = (regsIndex)indexOfRegister;
    stackPrint( &(SPU->stk) );
    regsPrint( SPU );
}
void doPushr( Processor* SPU ){
    int indexOfRegister = (SPU->code).command[ ++(SPU->instructionPointer) ];
    SPU->indexForRegister = (regsIndex)( indexOfRegister );
    stackPush( &(SPU->stk), (SPU->regs)[ indexOfRegister ] );
    stackPrint( &(SPU->stk) );
    regsPrint( SPU );
}
void doJB( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    int first = stackPop( &(SPU->stk) );
    if( first < last ){
        SPU->instructionPointer = (SPU->code).command[ (SPU->instructionPointer) + 1 ] - 1;
    }
    else if( SPU->instructionPointer < ( (SPU->code).sizeOfCommands - 1) ){
        SPU->instructionPointer += 1;
        //printf("\nElement in code [%lu] = %d\n", SPU->instructionPointer, (SPU->code).command[SPU->instructionPointer]);
    }
    colorPrintf( NOMODE, PURPLE, "\nInstruction pointer = %lu\n", SPU->instructionPointer );
}

void doJAE( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    int first = stackPop( &(SPU->stk) );
    if( first >= last ){
        SPU->instructionPointer = (SPU->code).command[ (SPU->instructionPointer) + 1 ] - 1;
    }
    else if( SPU->instructionPointer < ( (SPU->code).sizeOfCommands - 1) ){
        SPU->instructionPointer += 1;
        //printf("\nElement in code from JA [%lu] = %d\n", SPU->instructionPointer, (SPU->code).command[SPU->instructionPointer]);
    }
    colorPrintf( NOMODE, PURPLE, "\nInstruction pointer = %lu\n", SPU->instructionPointer );
}

void doCall( Processor* SPU ){
    if( SPU->instructionPointer + 2 >= (SPU->code).sizeOfCommands ){
        return ;
    }

    int indexOfNextCommand = SPU->instructionPointer + 2;
    SPU->instructionPointer = (SPU->code).command[ SPU->instructionPointer + 1 ] - 1;
    stackPush( &(SPU->regAddr), indexOfNextCommand );
}

void doRet( Processor *SPU ){
    int indexOfReturnCommand = stackPop( &(SPU->regAddr) );
    SPU->instructionPointer = indexOfReturnCommand - 1;
}

void doPopm( Processor* SPU ){
    size_t ramIndex = (SPU->code).command[ ++(SPU->instructionPointer) ];
    colorPrintf(NOMODE, RED, "\nPOPM index = %d\n", ramIndex );
    (SPU->RAM)[ (SPU->regs)[ramIndex] ] = stackPop( &(SPU->stk) );
    stackPrint( &(SPU->stk) );
}

void doPushm( Processor* SPU ){
    size_t ramIndex = (SPU->code).command[ ++(SPU->instructionPointer) ];
    colorPrintf(NOMODE, RED, "\nPUSHM index = %d\n", ramIndex );
    stackPush( &(SPU->stk), (SPU->RAM)[ (SPU->regs)[ramIndex] ] );
    stackPrint( &(SPU->stk) );
}
