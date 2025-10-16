#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "assembleCode.h"
#include "stringFunction.h"
#include "paint.h"

commandForPrint arrayWithOneCommand[] = {
    { "PUSH",   1 },
    { "IN",     9 },
    { "MUL",    2 },
    { "SUB",    3 },
    { "ADD",    5 },
    { "OUT",    4 },
    { "HLT",    0 },
    { "PUSHR", 33 },
    { "POPR",  42 },
    { "PUSHM", 70 },
    { "POPM",  71 },
    { "[AX]",  RAX },
    { "[BX]",  RBX },
    { "[CX]",  RCX },
    { "[DX]",  RDX },
    { "RAX",  RAX },
    { "RBX",  RBX },
    { "RCX",  RCX },
    { "RDX",  RDX },
    { "RET",  61 }
};
size_t sizeArrayWithOneCommand = sizeof( arrayWithOneCommand ) / sizeof( arrayWithOneCommand[ 0 ] );

commandForLabel arrayWithLabelCommand[] = {
    { "JB", 50,  checkLabel },
    { "JAE", 53, checkLabel },
    { "CALL", 60, checkLabel },
};
size_t sizeArrayWithLabelCommand = sizeof( arrayWithLabelCommand ) / sizeof( arrayWithLabelCommand[ 0 ] );

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode, int* labels){
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

    err = splitToOperands( stringFromFile, &stringCommand );
    if( err != OK ){
        return err;
    }

    int* commandArray = (int*)calloc( stringCommand.arraySize, sizeof( int ) );
    if( commandArray == NULL ){
        return NULL_PTR;
    }

    FILE* byteFile = fopen( fileForByteCode, "a" );
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }

    bool doSecondPass = false;
    size_t sizeOfEncodeCommands = encodeCommands( stringCommand, byteFile, labels, commandArray, &doSecondPass );

    if( doSecondPass ){
        sizeOfEncodeCommands = encodeCommands( stringCommand, byteFile, labels, commandArray, &doSecondPass );
    }

    err = printInFile( byteFile, commandArray, sizeOfEncodeCommands );
    if( err != OK ){
        return err;
    }

    fclose( byteFile );
    free( commandArray );
    destroyStringCommand( &stringCommand );

    return OK;
}

void parseCommandName( char* lineFromFile ){
    assert( lineFromFile != NULL );

    char* getIndexOfComment = strchr( lineFromFile, ';' );
    if( getIndexOfComment != NULL ){
        *getIndexOfComment = '\0';
    }
}

void asmPrintf( FILE* byteFile,  int intFirstArg){
    fprintf( byteFile, "%d ", intFirstArg);
}

typeOfErr splitToOperands( strInformation stringFromFile, informationOfStringCommand* stringCommand ){
    if( stringCommand == NULL ){
        colorPrintf( NOMODE, RED, "\nstruct stringCommand = NULL PTR in :%s %s %d\n", __FILE__, __func__, __LINE__ );
        return COMMAND_NULL_PTR;
    }

    int statusOfReading = -1;
    char commandName[ lenOfCommand ] = "", commandCode[ lenOfCommand ] = "";

    for( size_t index = 0; index < stringFromFile.arraySize; index++ ){
        parseCommandName( stringFromFile.arrayOfStr[ index ] );

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

    printf("\n\n___________print__from__splitToOperands_____\n\n");
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
    stringCommand->arrayWithStringCommand = (char**)calloc( stringCommand->startSize, sizeof( char* ) );
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

size_t encodeCommands( informationOfStringCommand stringFromFile, FILE* byteFile, int* labels, int* commandArray, bool* doSecondPass ){
    assert( byteFile != NULL );

    bool flag = false;
    size_t stringIndex = 0, intIndex = 0, index = 0;
    printf("______________________\n");
    while( stringIndex < stringFromFile.arraySize){
        flag = false;
        printf("%s\n", (stringFromFile.arrayWithStringCommand)[ stringIndex ] );

        flag = checkNumber( stringFromFile, &stringIndex, commandArray, &intIndex);
        GO_NEXT_OR_NOT();

        flag = findLabel( stringFromFile, &stringIndex, labels, commandArray, &intIndex );
        GO_NEXT_OR_NOT();

        for( index = 0; index < sizeArrayWithLabelCommand; index++ ){
            if( strcmp( (stringFromFile.arrayWithStringCommand)[ stringIndex ], arrayWithLabelCommand[ index ].firstArg ) == 0 ){
                commandArray[ intIndex++ ] = arrayWithLabelCommand[ index ].intFirstArg;
                ++stringIndex;
                (*doSecondPass) = arrayWithLabelCommand[ index ].func( stringFromFile, &stringIndex, labels, commandArray, &intIndex );
                flag = true;
                break;
            }
        }
        GO_NEXT_OR_NOT();

        for( index = 0; index < sizeArrayWithOneCommand; index++ ){
            if( strcmp( (stringFromFile.arrayWithStringCommand)[ stringIndex ], arrayWithOneCommand[ index ].firstArg ) == 0 ){
                commandArray[ intIndex++ ] = arrayWithOneCommand[ index ].intFirstArg;
                ++stringIndex;
                flag = true;
                break;
            }

        }
        GO_NEXT_OR_NOT();
        ++stringIndex;
    }

    return intIndex;
}

bool checkNumber( informationOfStringCommand stringFromFile, size_t* stringIndex, int* commandArray, size_t* intIndex ){
    if( stringIndex == NULL ){
        return false;
    }
    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];

    if( isdigit( command[0]) || command[0] == '-' ){
        commandArray[ (*intIndex)++ ] = atoi( command );
        ++(*stringIndex);
        return true;
    }

    return false;
}

bool checkLabel( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex ){
    assert( stringIndex != NULL );
    assert( labels != NULL );
    assert( commandArray != NULL );
    assert( intIndex != NULL );

    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];

    commandArray[ (*intIndex)++ ] = labels[ atoi( command + 1 ) ];

    if( *stringIndex < stringFromFile.arraySize - 1 ){
        ++(*stringIndex);
    }
    if( labels[ atoi( command + 1 ) ] == -1 ){
        return true;
    }

    return false;
}

bool findLabel( informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* commandArray, size_t* intIndex ){
    char* command = (stringFromFile.arrayWithStringCommand)[ *stringIndex ];
    if( command[0] == ':' ){
        labels[ atoi( command + 1 ) ] = *intIndex;
        ++(*stringIndex);
        return true;
    }

    return false;
}
typeOfErr printInFile( FILE* byteFile, int* commandArray, size_t sizeOfIntCommand ){
    if( byteFile == NULL ){
        return BYTE_FILE_ERROR;
    }
    else if( commandArray == NULL ){
        return NULL_PTR;
    }

    for( size_t index = 0; index < sizeOfIntCommand; index++ ){
        fprintf( byteFile, "%d ", commandArray[ index ] );
    }
    return OK;
}
