#include <stdio.h>

#include "assembleCode.h"
#include "paint.h"

int main( int argc, char** argv){


    typeOfErr codeWithError = assemble( argv[1], argv[2] );

    if( codeWithError != OK ){
        colorPrintf( NOMODE, RED, "\nError from assemble :%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    return 0;
}
