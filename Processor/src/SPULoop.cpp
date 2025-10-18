#include <stdio.h>
#include <string.h>
#include <cmath>

#include "SPULoop.h"
#include "softProcessor.h"
#include "paint.h"
#include "checkError.h"

calculatorErrors calculationFromProcessor( Processor *SPU, const char* byteFile ){
    SPU_OK( SPU, ERROR_FROM_SPU );

    softProcessor( byteFile, SPU );
    int DO = 1;
    calculatorErrors typeOfErr = SUCCESSFUL;

    while( (SPU->instructionPointer) < (SPU->code).sizeOfCommands && DO ){
        switch( (SPU->code).command[ SPU->instructionPointer ] ){
            case HLT:
                DO = 0;
                break;
            case PUSH:
                doPush( SPU );
                break;
            case MUL:
                typeOfErr = doMathOperation( SPU, mulNumbers );
                checkCalculatorStatus( typeOfErr );
                break;
            case SUB:
                typeOfErr = doMathOperation( SPU, subNumbers );
                checkCalculatorStatus( typeOfErr );
                break;
            case ADD:
                typeOfErr = doMathOperation( SPU, sumNumbers );
                checkCalculatorStatus( typeOfErr );
                break;
            case DIV:
                typeOfErr = doMathOperation( SPU, divNumbers );
                checkCalculatorStatus( typeOfErr );
                break;
            case MOD:
                typeOfErr = doMathOperation( SPU, modNumbers );
                checkCalculatorStatus( typeOfErr );
                break;
            case OUT:
                doOut( SPU );
                break;
            case SQRT:
                typeOfErr = doSqrt( SPU );
                checkCalculatorStatus( typeOfErr );
                break;
            case IN:
                doIn( SPU );
                break;
            case POPR:
                typeOfErr = doPopr( SPU );
                checkCalculatorStatus( typeOfErr );
                break;
            case PUSHR:
                doPushr( SPU );
                break;
            case JB:
                typeOfErr = doJB( SPU );
                checkCalculatorStatus( typeOfErr );
                break;
            case JAE:
                typeOfErr = doJAE( SPU );
                checkCalculatorStatus( typeOfErr );
                break;
            case CALL:
                doCall( SPU );
                break;
            case RET:
                typeOfErr = doRet( SPU );
                checkCalculatorStatus( typeOfErr );
                break;
            case PUSHM:
                doPushm( SPU );
                break;
            case POPM:
                typeOfErr = doPopm( SPU );
                checkCalculatorStatus( typeOfErr );
                break;
            case DRAW:
                ramPrint( SPU );
                break;
            default:
                break;
        }
        ++(SPU->instructionPointer);
    }

    return typeOfErr;
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
int modNumbers( int first, int last ){
    if( last != 0 ){
        return first % last ;
    }
    return (int)0;
}
calculatorErrors doMathOperation( Processor* SPU, int( *mathFunction )( int first, int last)){
    int last = stackPop( &(SPU->stk) );
    int first = stackPop( &(SPU->stk) );
    if( last == poison_ || first == poison_ ){
        colorPrintf( NOMODE, RED, "\n\nNot enough elements in stack\n\n");
        return FEW_ELEMENTS;
    }
    stackPush( &(SPU->stk), mathFunction( first, last ) );
    stackPrint( &(SPU->stk) );
    return SUCCESSFUL;
}
void doOut( Processor* SPU ){
    printf("%d\n", stackPop( &(SPU->stk ) ) );
    stackPrint( &(SPU->stk) );
}
calculatorErrors doSqrt( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    if( last == poison_ ){
        colorPrintf( NOMODE, RED, "\n\nNot enough elements in stack\n\n");
        return FEW_ELEMENTS;
    }
    else if( last >= 0 ){
        printf("%d\n", (int)sqrt( last ) );
        stackPrint( &(SPU->stk) );
        return SUCCESSFUL;
    }
    colorPrintf( NOMODE, RED, "\n\nThe root of a negative numbers\n\n");
    return THE_ROOT_OF_NEGATIVE_NUMBERS;
}
void doIn( Processor* SPU ){
    int number = 0;
    colorPrintf( NOMODE, BLUE, "Enter radius from keyboard: " );
    scanf("%d", &number );
    //stackPush( &(SPU->stk), first );
    (SPU->regs)[ RDX ] = number;
    stackPrint( &(SPU->stk) );
}
calculatorErrors doPopr( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    if( last == poison_ ){
        colorPrintf( NOMODE, RED, "\n\nNot enough elements in stack\n\n");
        return FEW_ELEMENTS;
    }
    int indexOfRegister = (SPU->code).command[ ++(SPU->instructionPointer) ];
    (SPU->regs)[ indexOfRegister ] = last;
    (SPU->indexForRegister) = (regsIndex)indexOfRegister;
    stackPrint( &(SPU->stk) );
    regsPrint( SPU );
    return SUCCESSFUL;
}
void doPushr( Processor* SPU ){
    int indexOfRegister = (SPU->code).command[ ++(SPU->instructionPointer) ];
    SPU->indexForRegister = (regsIndex)( indexOfRegister );
    stackPush( &(SPU->stk), (SPU->regs)[ indexOfRegister ] );
    stackPrint( &(SPU->stk) );
    regsPrint( SPU );
}
calculatorErrors doJB( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    int first = stackPop( &(SPU->stk) );
    if( first == poison_ || last == poison_ ){
        colorPrintf( NOMODE, RED, "\n\nNot enough elements in stack\n\n");
        return FEW_ELEMENTS;
    }
    if( first < last ){
        SPU->instructionPointer = (SPU->code).command[ (SPU->instructionPointer) + 1 ] - 1;
    }
    else if( SPU->instructionPointer < ( (SPU->code).sizeOfCommands - 1) ){
        SPU->instructionPointer += 1;
        //printf("\nElement in code [%lu] = %d\n", SPU->instructionPointer, (SPU->code).command[SPU->instructionPointer]);
    }
    colorPrintf( NOMODE, PURPLE, "\nInstruction pointer = %lu\n", SPU->instructionPointer );
    return SUCCESSFUL;
}

calculatorErrors doJAE( Processor* SPU ){
    int last = stackPop( &(SPU->stk) );
    int first = stackPop( &(SPU->stk) );
    if( first == poison_ || last == poison_ ){
        colorPrintf( NOMODE, RED, "\n\nNot enough elements in stack\n\n");
        return FEW_ELEMENTS;
    }
    if( first >= last ){
        SPU->instructionPointer = (SPU->code).command[ (SPU->instructionPointer) + 1 ] - 1;
    }
    else if( SPU->instructionPointer < ( (SPU->code).sizeOfCommands - 1) ){
        SPU->instructionPointer += 1;
        //printf("\nElement in code from JA [%lu] = %d\n", SPU->instructionPointer, (SPU->code).command[SPU->instructionPointer]);
    }
    colorPrintf( NOMODE, PURPLE, "\nInstruction pointer = %lu\n", SPU->instructionPointer );
    return SUCCESSFUL;
}

void doCall( Processor* SPU ){
    if( SPU->instructionPointer + 2 >= (SPU->code).sizeOfCommands ){
        return ;
    }

    int indexOfNextCommand = SPU->instructionPointer + 2;
    SPU->instructionPointer = (SPU->code).command[ SPU->instructionPointer + 1 ] - 1;
    stackPush( &(SPU->regAddr), indexOfNextCommand );
}

calculatorErrors doRet( Processor *SPU ){
    int indexOfReturnCommand = stackPop( &(SPU->regAddr) );
    if( indexOfReturnCommand == poison_ ){
        colorPrintf( NOMODE, RED, "\n\nNot enough elements in stack\n\n");
        return FEW_ELEMENTS;
    }
    SPU->instructionPointer = indexOfReturnCommand - 1;
    return SUCCESSFUL;
}

calculatorErrors doPopm( Processor* SPU ){
    size_t ramIndex = (SPU->code).command[ ++(SPU->instructionPointer) ];
    colorPrintf(NOMODE, RED, "\nPOPM index = %d\n", ramIndex );
    int ramElement = stackPop( &(SPU->stk ) );
    if( ramElement == poison_ ){
        colorPrintf( NOMODE, RED, "Not enough element in stack\n\n");
        return FEW_ELEMENTS;
    }
    (SPU->RAM)[ (SPU->regs)[ramIndex] ] = ramElement;
    stackPrint( &(SPU->stk) );
    return SUCCESSFUL;
}

void doPushm( Processor* SPU ){
    size_t ramIndex = (SPU->code).command[ ++(SPU->instructionPointer) ];
    colorPrintf(NOMODE, RED, "\nPUSHM index = %d\n", ramIndex );
    stackPush( &(SPU->stk), (SPU->RAM)[ (SPU->regs)[ramIndex] ] );
    stackPrint( &(SPU->stk) );
}
