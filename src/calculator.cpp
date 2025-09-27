#include <stdio.h>
#include <string.h>
#include <cmath>

#include "calculator.h"

void calculator( stack_t *stk ){
    char command[20] = "\0";
    int number = 0;
    type first = 0, last = 0;
    while(true){
        if ( scanf("%s", command ) != 1){
            continue;
        }
        if( strcmp( command, "PUSH" ) == 0 ){
            scanf("%d", &number );
            stackPush( stk, number );
        }
        else if( strcmp( command, "ADD") == 0 ){
            last = stackPop( stk );
            first = stackPop( stk );
            stackPush( stk, first + last );
        }
        else if( strcmp( command, "SUB")  == 0 ){
            last = stackPop( stk );
            first = stackPop( stk );
            stackPush( stk, first - last );
        }
        else if( strcmp( command, "DIV") == 0 ){
            last = stackPop( stk );
            first = stackPop( stk );
            if ( last != 0 ){
                stackPush( stk, first / last );
            }
        }
        else if( strcmp( command, "MUL") == 0 ){
            last = stackPop( stk );
            first = stackPop( stk );
            stackPush( stk, last * first );
        }
        else if( strcmp( command, "OUT") == 0 ){
            printf("%d\n", stackPop( stk ) );
        }
        else if( strcmp( command, "SQRT") == 0){
            last = stackPop( stk );
            if( last >= 0 ){
                printf("%d\n", (int)sqrt( last ) );
            }
        }
        else if( strcmp( command, "HLT") == 0 ){
            break;
        }

        stackPrint( stk );
    }
}