#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processor.h"
#include "onegin.h"


void ASM( const char* fileForAsm, const char* fileForByteCode ){ // TODO assembleCode
    FILE* asmFile = fopen( fileForAsm, "r");

    bufferInformation bufferFromFile = {};
    strInformation stringFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, asmFile );
    errorCode stringError = initStringInformation( &stringFromFile );
    // TODO check bufferError and stringError

    splitToLines( &bufferFromFile, &stringFromFile, asmFile );
    // TODO check splitToLines

    FILE* byteFile = fopen( fileForByteCode, "a" );

    size_t indexArray = 0, indexLine = 0;
    char codeForOperation[ 10 ] = "\0";
    for( ; indexArray < stringFromFile.arraySize; indexArray++ ){

        char* line = *( stringFromFile.arrayOfStr + indexArray);
        size_t sizeLine = strlen( line );

        while( line[ indexLine ] != ' ' && indexLine < sizeLine ){
            codeForOperation[ indexLine ] = line[ indexLine ];
            ++indexLine;
        }
        codeForOperation[ indexLine ] = '\0';

        if( strcmp( codeForOperation, "PUSH" ) == 0 ){
            ++indexLine;
            int number = atoi( line + indexLine );
            fprintf( byteFile, "1 %d\n", number );
        }
        else if( strcmp( codeForOperation, "MUL" ) == 0 ){
            fprintf( byteFile, "2\n" );
        }
        else if( strcmp( codeForOperation, "SUB" ) == 0 ){
            fprintf( byteFile, "3\n" );
        }
        else if( strcmp( codeForOperation, "OUT" ) == 0 ){
            fprintf( byteFile, "4\n" );
        }
        else if( strcmp( codeForOperation, "HLT" ) == 0 ){
            fprintf( byteFile, "0\n" );
        }

        indexLine = 0;
    }

    fclose( asmFile );
    fclose( byteFile );


}
