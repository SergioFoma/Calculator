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

};

void ASM_PUSH( FILE* byteFile,  const char* intFirstArg,
               size_t whitespaceIndex = 0, char* lineInArray = "\0");

void ASM_FPRINTF( FILE* byteFile,  const char* intFirstArg,
               size_t whitespaceIndex = 0, char* lineInArray = "\0");

struct commandInformation {
    const char* firstArg;
    const char* intFirstArg;
    void ( *func )( FILE* byteFile,  const char* intFirstArg,
                    size_t whitespaceIndex , char* lineInArray );
};

typeOfErr assemble( const char* fileForAsm, const char* fileForByteCode );

void parseCommandName( char* lineFromFile, char* codeForOperation, size_t* indexLine );

typeOfErr writeToFile( strInformation stringFromFile, FILE* byteFile );

#endif
