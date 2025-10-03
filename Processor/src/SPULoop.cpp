#include <stdio.h>
#include <string.h>
#include <cmath>

#include "SPULoop.h"

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
