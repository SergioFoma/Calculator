//#include <TXLib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "checkError.h"
#include "paint.h"
#include "SPULoop.h"
#include "softProcessor.h"

int main(int* argc, char** argv){

    Processor SPU = {};

    processorInit( &SPU );

    if( strcmp( argv[1], "--softProcessor" ) == 0 ){
        calculationFromProcessor( &SPU );
    }

    processorDestroy( &SPU );

    return 0;
}
