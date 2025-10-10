#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "assembleCode.h"
#include "onegin.h"
#include "paint.h"

commandInformation arrayWithAssembleCommand[] = {
    { "PUSH", "1", ASM_PUSH },
    { "JB", "50", ASM_PUSH },
    { "JA", "51", ASM_PUSH },
    { "IN", "9",  ASM_FPRINTF },
    { "MUL", "2", ASM_FPRINTF },
    { "SUB", "3", ASM_FPRINTF },
    { "ADD", "5", ASM_FPRINTF },
    { "OUT", "4", ASM_FPRINTF },
    { "HLT", "0",   ASM_FPRINTF },
    { "POPR RAX", "42 0",   ASM_FPRINTF },
    { "PUSHR RAX", "33 0",  ASM_FPRINTF },
    { "POPR RBX", "42 1",  ASM_FPRINTF },
    { "PUSHR RBX", "33 1",   ASM_FPRINTF },
    { "POPR RCX", "42 2",   ASM_FPRINTF },
    { "PUSHR RCX", "33 2",   ASM_FPRINTF },
    { "POPR RDX", "42 3",   ASM_FPRINTF },
    { "PUSHR RDX", "33 3",   ASM_FPRINTF },
    { "SOLVE EQUATION", "10",   ASM_FPRINTF }
};

size_t sizeArrayWithASMCommand = sizeof( arrayWithAssembleCommand ) / sizeof( arrayWithAssembleCommand[ 0 ] );

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode, int* labels, size_t* countOfCommand ){
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

    typeOfErr codeOfError = writeToFile( stringFromFile, byteFile, labels, countOfCommand );
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

typeOfErr writeToFile( strInformation stringFromFile, FILE* byteFile, int* labels, size_t* countOfCommand ){
    assert( byteFile != NULL );

    char codeForOperation[ lenOfCommand ] = "\0";
    size_t whitespaceIndex = 0, indexArray = 0;

    for( ; indexArray < stringFromFile.arraySize; indexArray++ ){

        bool flag = false;
        char* lineInArray = *(stringFromFile.arrayOfStr + indexArray);

        if ( findLabels( byteFile, lineInArray, labels, countOfCommand ) ){
            continue;
        }
        parseCommandName( lineInArray , codeForOperation, &whitespaceIndex );
        if( whitespaceIndex == strlen( lineInArray ) ){
            *countOfCommand += 1;
        }
        else{
            *countOfCommand += 2;
        }

        for( size_t indexASMArray = 0; indexASMArray < sizeArrayWithASMCommand; indexASMArray++ ){
            if( strcmp( codeForOperation, (arrayWithAssembleCommand[ indexASMArray]).firstArg ) == 0 &&
                indexASMArray <= 2 ){
                (arrayWithAssembleCommand[ indexASMArray ] ).func( byteFile, (arrayWithAssembleCommand[ indexASMArray]).intFirstArg,
                                                                   whitespaceIndex, lineInArray );
                flag = true;
                break;
            }
            else if( strcmp( codeForOperation, (arrayWithAssembleCommand[ indexASMArray]).firstArg ) == 0 ||
                     strcmp( lineInArray, (arrayWithAssembleCommand[ indexASMArray]).firstArg ) == 0){
                (arrayWithAssembleCommand[ indexASMArray ] ).func( byteFile, (arrayWithAssembleCommand[ indexASMArray]).intFirstArg,
                                                                   0, "");
                flag = true;
                break;
            }
        }

        if( flag == false && strlen( lineInArray ) > 0 ){
            colorPrintf( NOMODE, RED, "\nUnidentified command in ASM File :%s %s %d\n%s %d\n", __FILE__, __func__, __LINE__ , lineInArray,strlen( lineInArray ));
            return COMMAND_ERROR;
        }
        whitespaceIndex = 0;
    }

    return OK;

}

void ASM_PUSH( FILE* byteFile,  const char* intFirstArg,
               size_t whitespaceIndex, char* lineInArray){
    ++whitespaceIndex;
    int number = atoi( lineInArray + whitespaceIndex );
    fprintf( byteFile, "%s %d\n", intFirstArg, number );
}
void ASM_FPRINTF( FILE* byteFile,  const char* intFirstArg,
                  size_t whitespaceIndex, char* lineInArray){
    fprintf( byteFile, "%s\n", intFirstArg);
}

bool findLabels( FILE* byteFile, char* lineFromFile, int* labels, size_t* countOfCommand ){
    if( lineFromFile[0] == ':' ){
        labels[ atoi( lineFromFile + 1 ) ] = *countOfCommand;
        return true;
    }

    char* colon = strchr( lineFromFile, ':' );
    if( colon != NULL ){
        int indexToJump = labels[ atoi( colon + 1 ) ];
        char* whitespace = strchr( lineFromFile, ' ' );
        *whitespace = '\0';
        for( size_t index = 0; index < sizeArrayWithASMCommand; index++ ){
            if( strcmp( lineFromFile, arrayWithAssembleCommand[index].firstArg ) == 0 ){
                fprintf( byteFile, "%s %d\n", arrayWithAssembleCommand[index].intFirstArg, indexToJump );
            }
        }
        *countOfCommand += 2;
        return true;
    }

    return false;
}
