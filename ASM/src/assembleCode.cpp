#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "assembleCode.h"
#include "onegin.h"
#include "paint.h"


typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode ){
    FILE* asmFile = fopen( fileForAsm, "r");

    bufferInformation bufferFromFile = {};
    strInformation stringFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, asmFile );
    if( bufferError != correct ){
        return INIT_BUFFER_ERROR;
    }

    errorCode stringError = initStringInformation( &stringFromFile );
    if( stringError != correct ){
        return INIT_STRING_ERROR;
    }

    if ( !splitToLines( &bufferFromFile, &stringFromFile, asmFile ) ){
        return SPLIT_LINES_ERROR;
    }
    fclose( asmFile );

    FILE* byteFile = fopen( fileForByteCode, "a" );
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }

    typeOfErr codeOfError = writeToFile( stringFromFile, byteFile );
    if( codeOfError != OK ){
        return COMMAND_ERROR;
    }
    fclose( byteFile );

    return OK;
}

void parseCommandName( char* lineFromFile, char* codeForOperation, size_t* whitespaceIndex ){
    assert( lineFromFile != NULL );
    assert( codeForOperation != NULL );
    assert( whitespaceIndex != NULL );

    char* getIndexOfComment = strchr( lineFromFile, ';' );
    if( getIndexOfComment != NULL ){
        *getIndexOfComment = '\0';
    }

    size_t sizeLine = strlen( lineFromFile );

    while( lineFromFile[ *whitespaceIndex ] != ' ' && *whitespaceIndex < sizeLine ){
        codeForOperation[ *whitespaceIndex ] = lineFromFile[ *whitespaceIndex ];
        ++(*whitespaceIndex);
    }
    codeForOperation[ *whitespaceIndex ] = '\0';
}

typeOfErr writeToFile( strInformation stringFromFile, FILE* byteFile ){
    assert( byteFile != NULL );

    char codeForOperation[ lenOfCommand ] = "\0";
    size_t whitespaceIndex = 0, indexArray = 0, countOfCommand = 0;

    for( ; indexArray < stringFromFile.arraySize; indexArray++ ){

        int number = 0;
        char* lineInArray = *(stringFromFile.arrayOfStr + indexArray);
        parseCommandName( lineInArray , codeForOperation, &whitespaceIndex );

        if( strcmp( codeForOperation, "PUSH" ) == 0 ){
            ++whitespaceIndex;
            number = atoi( lineInArray + whitespaceIndex );
            fprintf( byteFile, "1 %d\n", number );
        }
        else if( strcmp( codeForOperation, "IN" ) == 0 ){
            fprintf( byteFile, "9\n" );
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
        else if( strcmp( lineInArray, "POPR RAX " ) == 0 ){
            fprintf( byteFile, "42 0\n" );
        }
        else if( strcmp( lineInArray, "PUSHR RAX " ) == 0 ){
            fprintf( byteFile, "33 0\n" );
        }
        else if( strcmp( lineInArray, "POPR RBX" ) == 0 ){
            fprintf( byteFile, "42 1\n" );
        }
        else if( strcmp( lineInArray, "PUSHR RBX" ) == 0 ){
            fprintf( byteFile, "33 1\n" );
        }
        else if( strcmp( lineInArray, "POPR RCX" ) == 0 ){
            fprintf( byteFile, "42 2\n" );
        }
        else if( strcmp( lineInArray, "PUSHR RCX" ) == 0 ){
            fprintf( byteFile, "33 2\n" );
        }
        else if( strcmp( lineInArray, "POPR RDX" ) == 0 ){
            fprintf( byteFile, "42 3\n" );
        }
        else if( strcmp( lineInArray, "PUSHR RDX" ) == 0 ){
            fprintf( byteFile, "33 3\n" );
        }
        else if( strcmp( lineInArray, "SOLVE EQUATION" ) == 0 ){
            fprintf( byteFile, "10\n" );
        }
        else if( strlen( lineInArray) > 0 ){
            colorPrintf( NOMODE, RED, "\nUnidentified command in ASM File :%s %s %d\n", __FILE__, __func__, __LINE__ );
            return COMMAND_ERROR;
        }
        whitespaceIndex = 0;
    }

    return OK;

}
