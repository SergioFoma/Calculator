//#include <TXLib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "checkError.h"
#include "paint.h"
#include "SPULoop.h"
#include "softProcessor.h"

int main(int* argc, char** argv){

    stack_t stk = {};
    size_t startSize = 8;

    stackInit( &stk, startSize );

    if( strcmp( argv[1], "--softProcessor" ) == 0 ){
        calculationFromProcessor( &stk );
    }
    else if( strcmp( argv[1], "--SPULoop" ) == 0 ){
        SPULoop( &stk );
    }

    stackDestroy( &stk );
    return 0;
}
