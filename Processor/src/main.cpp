//#include <TXLib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "checkError.h"
#include "paint.h"
#include "SPULoop.h"
#include "softProcessor.h"

int main(int* argc, char** argv){

    if( strcmp( argv[1], "--softProcessor" ) == 0 ){
        Code informationOfCommand = {};

        softProcessor( "BYTE-CODE.txt", &informationOfCommand );

        for( size_t index = 0; index < informationOfCommand.sizeOfCommands; index++ ){
            printf("%d ", informationOfCommand.command[ index ] );
        }
        printf("\n");
        free( informationOfCommand.command );
    }
    else if( strcmp( argv[1], "--SPULoop" ) == 0 ){
        stack_t stk = {};
        size_t startSize = 8;

        stackInit( &stk, startSize );

        SPULoop( &stk );

        stackDestroy( &stk );
    }

    return 0;
}
