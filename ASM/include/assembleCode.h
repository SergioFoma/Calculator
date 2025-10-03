#ifndef H_ASSEMBLECODE
#define H_ASSEMBLECODE

#include <stdio.h>

#include "onegin.h"

const size_t lenOfCommand = 20;

void assemble( const char* fileForAsm, const char* fileForByteCode );

void getCommand( char* lineFromFile, char* codeForOperation, size_t* indexLine );

void writeToFile( strInformation stringFromFile, FILE* byteFile );

#endif
