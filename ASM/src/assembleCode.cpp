#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "assembleCode.h"
#include "onegin.h"
#include "paint.h"

commandForPrint arrayWithOneCommand[] = {
    { "PUSH", 1, ASM_FPRINTF },
    { "IN", 9,  ASM_FPRINTF },
    { "MUL", 2, ASM_FPRINTF },
    { "SUB", 3, ASM_FPRINTF },
    { "ADD", 5, ASM_FPRINTF },
    { "OUT", 4, ASM_FPRINTF },
    { "HLT", 0,   ASM_FPRINTF },
    { "PUSHR", 33, ASM_FPRINTF },
    { "POPR", 42, ASM_FPRINTF },
    { "JB", 50, ASM_FPRINTF },
    { "JAE", 53, ASM_FPRINTF },
    { "CALL", 60, ASM_FPRINTF},
    { "RET", 61, ASM_FPRINTF },
    { "RAX", RAX, ASM_FPRINTF },
    { "RBX", RBX, ASM_FPRINTF },
    { "RCX", RCX, ASM_FPRINTF},
    { "RDX", RDX, ASM_FPRINTF }
};
size_t sizeArrayWithOneCommand = sizeof( arrayWithOneCommand ) / sizeof( arrayWithOneCommand[ 0 ] );

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode, int* labels, bool writeOrNot){
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

    informationOfStringCommand stringCommand = {};
    typeOfErr err = initStringCommand( &stringCommand );
    if( err != OK ){
        return err;
    }

    size_t sizeOfIntCommands = 0;
    err = takeCommandArray( stringFromFile, &stringCommand, &sizeOfIntCommands );
    if( err != OK ){
        return err;
    }

    int* arrayWithIntCommand = (int*)calloc( sizeOfIntCommands, sizeof( int ) );
    if( arrayWithIntCommand == NULL ){
        return NULL_PTR;
    }

    FILE* byteFile = fopen( fileForByteCode, "a" );
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }

    err = takeIntArray( stringCommand, byteFile, labels, arrayWithIntCommand );
    if( err != OK ){
        return err;
    }

    if( writeOrNot ){
        err = takeIntArray( stringCommand, byteFile, labels, arrayWithIntCommand );
    }

    err = printInFile( byteFile, arrayWithIntCommand, sizeOfIntCommands );
    if( err != OK ){
        return err;
    }

    fclose( byteFile );

    free( arrayWithIntCommand );

    destroyStringCommand( &stringCommand );

    return OK;
}

bool parseCommandName( char* lineFromFile ){
    assert( lineFromFile != NULL );

    char* getIndexOfComment = strchr( lineFromFile, ';' );
    if( getIndexOfComment != NULL ){
        *getIndexOfComment = '\0';
    }

    if( lineFromFile[0] == ':' && lineFromFile[1] != ':'){
        return true;
    }

    return false;
}

void ASM_FPRINTF( FILE* byteFile,  int intFirstArg){
    fprintf( byteFile, "%d ", intFirstArg);
}

typeOfErr takeCommandArray( strInformation stringFromFile, informationOfStringCommand* stringCommand, size_t* sizeOfIntCommands ){
    if( stringCommand == NULL ){
        colorPrintf( NOMODE, RED, "\nstruct stringCommand = NULL PTR in :%s %s %d\n", __FILE__, __func__, __LINE__ );
        return COMMAND_NULL_PTR;
    }

    if( stringCommand->arrayWithStringCommand == NULL ){
        stringCommand->arrayWithStringCommand = (char**)calloc( stringCommand->startSize, sizeof( char* ) );
    }
    int statusOfReading = -1;
    size_t countOfLabels = 0;
    char commandName[ lenOfCommand ] = "", commandCode[ lenOfCommand ] = "";

    for( size_t index = 0; index < stringFromFile.arraySize; index++ ){
        if( parseCommandName( stringFromFile.arrayOfStr[ index ] ) ){
            ++countOfLabels;
        }
        statusOfReading = sscanf( stringFromFile.arrayOfStr[ index ], "%s %s", commandName, commandCode );
        if( (stringCommand->arraySize + statusOfReading) >= stringCommand->startSize ){
            stringCommand->startSize *= 2;
            stringCommand->arrayWithStringCommand= (char**)realloc( stringCommand->arrayWithStringCommand,
                                                      (stringCommand->startSize) * sizeof( char* ) );
            if( stringCommand->arrayWithStringCommand == NULL ){
                return COMMAND_NULL_PTR;
            }
        }
        if( statusOfReading == 1 ){
            (stringCommand->arrayWithStringCommand)[ (stringCommand->arraySize)++ ] = strdup(commandName);
        }
        else if( statusOfReading == 2 ){
            (stringCommand->arrayWithStringCommand)[ (stringCommand->arraySize)++ ] = strdup(commandName);
            (stringCommand->arrayWithStringCommand)[ (stringCommand->arraySize)++ ] = strdup(commandCode);
        }
    }

    *sizeOfIntCommands = stringCommand->arraySize - countOfLabels;
    for( size_t index = 0; index<(stringCommand->arraySize); index++ ){
        printf("'%s'\n", (stringCommand->arrayWithStringCommand)[ index ] );
    }
    return OK;
}

typeOfErr initStringCommand( informationOfStringCommand* stringCommand ){
    if( stringCommand == NULL ){
        return INIT_COMMAND_ERROR;
    }

    stringCommand->startSize = 8;
    stringCommand->arrayWithStringCommand = NULL;
    stringCommand->arraySize = 0;
    return OK;
}

typeOfErr destroyStringCommand( informationOfStringCommand* stringCommand ){
    if( stringCommand == NULL ){
        return COMMAND_NULL_PTR;
    }

    for( size_t index = 0; index < (stringCommand->arraySize); index++){
        free( (stringCommand->arrayWithStringCommand)[ index ] );
    }

    free( stringCommand->arrayWithStringCommand );
    stringCommand->arraySize = 0;
    stringCommand->startSize = 0;
    return OK;
}

typeOfErr takeIntArray( informationOfStringCommand stringFromFile, FILE* byteFile, int* labels, int* arrayWithIntCommand ){
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }

    bool flag = false;
    size_t stringIndex = 0, intIndex = 0, index = 0;
    printf("______________________\n");
    while( stringIndex < stringFromFile.arraySize){
        flag = false;
        printf("%s\n", (stringFromFile.arrayWithStringCommand)[ stringIndex ] );

        flag = checkNumber( byteFile, stringFromFile, &stringIndex, arrayWithIntCommand, &intIndex);
        GO_NEXT_OR_NOT();

        flag = checkLabel( byteFile, stringFromFile, &stringIndex, labels, arrayWithIntCommand, &intIndex );
        GO_NEXT_OR_NOT();

        for( index = 0; index < sizeArrayWithOneCommand; index++ ){
            if( strcmp( (stringFromFile.arrayWithStringCommand)[ stringIndex ], arrayWithOneCommand[ index ].firstArg ) == 0 ){
                arrayWithIntCommand[ intIndex++ ] = arrayWithOneCommand[ index ].intFirstArg;
                ++stringIndex;
                flag = true;
                break;
            }

        }
        GO_NEXT_OR_NOT();
        ++stringIndex;
    }

    return OK;
}

bool checkNumber( FILE* byteFile, informationOfStringCommand stringFromFile, size_t* stringIndex, int* arrayWithIntCommand, size_t* intIndex ){
    if( stringIndex == NULL || byteFile == NULL ){
        return false;
    }
    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];

    if( isdigit( command[0]) || command[0] == '-' ){
        arrayWithIntCommand[ (*intIndex)++ ] = atoi( command );
        ++(*stringIndex);
        return true;
    }

    return false;
}

bool checkLabel( FILE* byteFile, informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* arrayWithIntCommand, size_t* intIndex ){
    if( byteFile == NULL || stringIndex == NULL || labels == NULL ){
        return false;
    }
    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];

    if( command[0] == ':' && command[1] == ':' ){
        arrayWithIntCommand[ (*intIndex)++ ] = labels[ atoi( command + 2 ) ];
        ++(*stringIndex);
        return true;
    }
    else if( command[0] == ':' ){
        labels[ atoi( command + 1 ) ] = *intIndex;
        ++(*stringIndex);
        return true;
    }

    return false;
}

typeOfErr printInFile( FILE* byteFile, int* arrayWithIntCommand, size_t sizeOfIntCommand ){
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }
    else if( arrayWithIntCommand == NULL ){
        return NULL_PTR;
    }

    for( size_t index = 0; index < sizeOfIntCommand; index++ ){
        fprintf( byteFile, "%d ", arrayWithIntCommand[ index ] );
    }
    return OK;
}
