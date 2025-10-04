#include <stdio.h>
#include <string.h>
#include <cmath>

#include "SPULoop.h"
#include "softProcessor.h"

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

    softProcessor( "BYTE-CODE.txt", SPU );
    size_t index = 0;
    int first = 0, last = 0, indexOfRegister = 0, DO = 1;

    while( index < (SPU->code).sizeOfCommands && DO ){
        switch( (SPU->code).command[ index ] ){
            case 0:
                DO = 0;
                break;
            case 1:
                stackPush( &(SPU->stk), (SPU->code).command[ ++index ] );
                stackPrint( &(SPU->stk) );
                break;
            case 2:
                last = stackPop( &(SPU->stk) );
                first = stackPop( &(SPU->stk) );
                stackPush( &(SPU->stk), first * last );
                stackPrint( &(SPU->stk) );
                break;
            case 3:
                last = stackPop( &(SPU->stk) );
                first = stackPop( &(SPU->stk) );
                stackPush( &(SPU->stk), first - last );
                stackPrint( &(SPU->stk) );
                break;
            case 4:
                printf("%d\n", stackPop( &(SPU->stk ) ) );
                stackPrint( &(SPU->stk) );
                break;
            case 5:
                last = stackPop( &(SPU->stk) );
                first = stackPop( &(SPU->stk) );
                stackPush( &(SPU->stk) , first + last );
                stackPrint( &(SPU->stk) );
                break;
            case 6:
                last = stackPop( &(SPU->stk) );
                first = stackPop( &(SPU->stk) );
                if ( last != 0 ){
                    stackPush( &(SPU->stk), first / last );
                }
                stackPrint( &(SPU->stk) );
                break;
            case 7:
                last = stackPop( &(SPU->stk) );
                if( last >= 0 ){
                    printf("%d\n", (int)sqrt( last ) );
                }
                stackPrint( &(SPU->stk) );
                break;
            case 42:
                last = stackPop( &(SPU->stk) );
                indexOfRegister = (SPU->code).command[ ++index ];
                (SPU->regs)[ indexOfRegister ] = last;
                stackPrint( &(SPU->stk) );
                regsPrint( SPU );
                break;
            case 33:
                indexOfRegister = (SPU->code).command[ ++index ];
                stackPush( &(SPU->stk), (SPU->regs)[ indexOfRegister ] );
                stackPrint( &(SPU->stk) );
                regsPrint( SPU );
                break;
        }
        ++index;
    }

}
