#ifndef H_ASSEMBLECODE
#define H_ASSEMBLECODE

#include <stdio.h>

#include "onegin.h"

const size_t lenOfCommand = 20;

enum typeOfErr {
    OK                 = 0,
    COMMAND_ERROR      = 1,
    BYTE_FILE_ERROR    = 2,
    SPLIT_LINES_ERROR  = 3,
    INIT_STRING_ERROR  = 4,
    INIT_BUFFER_ERROR  = 5,
    INIT_COMMAND_ERROR = 6,
    COMMAND_NULL_PTR   = 7,
};
enum regsIndex {
    RAX             = 0,
    RBX             = 1,
    RCX             = 2,
    RDX             = 3,
    NOT_IDENTIFIED  = 9
};

void ASM_FPRINTF( FILE* byteFile,  int intFirstArg );

struct commandForPrint {
    const char* firstArg;
    int intFirstArg;
    void ( *func )( FILE* byteFile,  int intFirstArg);
};

struct informationOfStringCommand {
    char** arrayWithStringCommand;
    int* arrayWithIntCommand;
    size_t arraySize;
    size_t startSize;
};

typeOfErr initStringCommand( informationOfStringCommand* stringCommand );

typeOfErr destroyStringCommand( informationOfStringCommand* stringCommand );

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode, int* labels, bool writeOrNot );

typeOfErr takeCommandArray( strInformation stringFromFile, informationOfStringCommand* stringCommand, size_t* sizeOfIntCommands );

typeOfErr takeIntArray( informationOfStringCommand stringFromFile, FILE* byteFile, int* labels, int* arrayWithIntCommand );

bool parseCommandName( char* lineFromFile );

typeOfErr writeToFile( strInformation stringFromFile, FILE* byteFile, int* labels, size_t* countOfCommand );

bool findLabels( FILE* byteFile, char* lineFromFile, int* labels, size_t* countOfCommand );

bool checkNumber( FILE* byteFile, informationOfStringCommand stringFromFile, size_t* stringIndex, int* arrayWithIntCommand, size_t* intIndex );

bool checkLabel( FILE* byteFile, informationOfStringCommand stringFromFile, size_t* stringIndex, int* labels, int* arrayWithIntCommand, size_t* intIndex  );

#define GO_NEXT_OR_NOT()            \
    if( flag == true ){             \
        continue;                   \
    }
#endif
