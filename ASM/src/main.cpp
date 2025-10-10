#include <stdio.h>

#include "assembleCode.h"
#include "paint.h"

int main( int argc, char** argv){

    const size_t labelsSize = 10;
    int labels[ labelsSize ] = {};
    size_t countOfCommand = 0;

    for( size_t index = 0; index < labelsSize; index++ ){
        labels[ index ] = -1;
    }

    typeOfErr codeWithError = assemble( argv[1], argv[2], labels, &countOfCommand );

    if( codeWithError != OK ){
        colorPrintf( NOMODE, RED, "\nError from assemble :%s %s %d\n", __FILE__, __func__, __LINE__ );
    }

    return 0;
}
