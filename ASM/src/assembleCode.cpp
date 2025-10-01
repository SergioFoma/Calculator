#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "assembleCode.h"
#include "onegin.h"
#include "paint.h"


void assemble( const char* fileForAsm, const char* fileForByteCode ){
    FILE* asmFile = fopen( fileForAsm, "r");

    bufferInformation bufferFromFile = {};
    strInformation stringFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, asmFile );
    if( bufferError != correct ){
        colorPrintf(NOMODE, RED, "Error of init buffer struct :%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ;
    }

    errorCode stringError = initStringInformation( &stringFromFile );
    if( stringError != correct ){
        colorPrintf(NOMODE, RED, "Error of init string struct :%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ;
    }

    if ( !splitToLines( &bufferFromFile, &stringFromFile, asmFile ) ){
        colorPrintf( NOMODE, RED, "Error of spit lines :%s %s %d\n", __FILE__, __func__, __LINE__ );
        return ;
    }

    FILE* byteFile = fopen( fileForByteCode, "a" );

    writeToFile( stringFromFile, byteFile );

    fclose( asmFile );
    fclose( byteFile );
}

void getCommand( char* lineFromFile, char* codeForOperation, size_t* indexLine ){
    assert( lineFromFile != NULL );
    assert( codeForOperation != NULL );
    assert( indexLine != NULL );

    char* getIndexOfComment = strchr( lineFromFile, ';' );
    if( getIndexOfComment != NULL ){
        *getIndexOfComment = '\0';
    }

    size_t sizeLine = strlen( lineFromFile );

    while( lineFromFile[ *indexLine ] != ' ' && *indexLine < sizeLine ){
        codeForOperation[ *indexLine ] = lineFromFile[ *indexLine ];
        ++(*indexLine);
    }
    codeForOperation[ *indexLine ] = '\0';
}

void writeToFile( strInformation stringFromFile, FILE* byteFile ){
    assert( byteFile != NULL );

    char codeForOperation[ lenOfCommand ] = "\0";
    size_t indexLine = 0, indexArray = 0;

    for( ; indexArray < stringFromFile.arraySize; indexArray++ ){

        getCommand( *(stringFromFile.arrayOfStr + indexArray), codeForOperation, &indexLine );

        if( strcmp( codeForOperation, "PUSH" ) == 0 ){
            ++indexLine;
            int number = atoi( *(stringFromFile.arrayOfStr + indexArray) + indexLine );
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

}
