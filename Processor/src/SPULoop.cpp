#include <stdio.h>
#include <string.h>
#include <cmath>

#include "SPULoop.h"
#include "softProcessor.h"
#include "paint.h"
#include "checkError.h"
#include "solver.h"

void SPULoop( stack_t *stk ){
    int code = 0, command = 0, DO = 1;
    type first = 0, last = 0;
    while( DO ){

        if ( scanf("%d", &code ) != 1){
            continue;
        }
        switch( code ){
            case 1:
                scanf("%d", &command );
                stackPush( stk, command );
                stackPrint( stk );
                break;
            case 5:
                last = stackPop( stk );
                first = stackPop( stk );
                stackPush( stk, first + last );
                stackPrint( stk );
                break;
            case 3:
                last = stackPop( stk );
                first = stackPop( stk );
                stackPush( stk, first - last );
                stackPrint( stk );
                break;
            case 6:
                last = stackPop( stk );
                first = stackPop( stk );
                if ( last != 0 ){
                    stackPush( stk, first / last );
                }
                stackPrint( stk );
                break;
            case 2:
                last = stackPop( stk );
                first = stackPop( stk );
                stackPush( stk, last * first );
                stackPrint( stk );
                break;
            case 4:
                printf("%d\n", stackPop( stk ) );
                stackPrint( stk );
                break;
            case 7:
                last = stackPop( stk );
                if( last >= 0 ){
                    printf("%d\n", (int)sqrt( last ) );
                }
                stackPrint( stk );
                break;
            case 0:
                DO = 0;
                break;
        }
    }
}

void calculationFromProcessor( Processor *SPU ){
    SPU_OK( SPU );

    softProcessor( "BYTE-CODE.txt", SPU );
    size_t index = 0;
    int first = 0, last = 0, indexOfRegister = 0, DO = 1;

    while( index < (SPU->code).sizeOfCommands && DO ){
        SPU->instructionPointer = index;
        switch( (SPU->code).command[ index ] ){
            case 0:
                DO = 0;
                break;
            case 1:
                DO_PUSH;
                break;
            case 2:
                DO_MUL;
                break;
            case 3:
                DO_SUB;
                break;
            case 4:
                DO_OUT;
                break;
            case 5:
                DO_ADD;
                break;
            case 6:
                DO_DIV;
                break;
            case 7:
                DO_SQRT;
                break;
            case 9:
                DO_IN;
                break;
            case 10:{
                DO_SOLVE_EQUATION;
                break;
            }
            case 42:
                DO_POPR;
                break;
            case 33:
                DO_PUSHR;
                break;
        }
        ++index;
    }

}
