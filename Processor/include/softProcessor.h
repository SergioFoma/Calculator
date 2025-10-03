#ifndef H_SOFTPROCESSOR
#define H_SOFTPROCESSOR

#include <stdio.h>

#include "onegin.h"

struct Code{
    int* command;
    size_t sizeOfCommands;
};

void softProcessor( const char* nameOfByteFile, Code* informationOfCommand );

void getArrayWithCommand( strInformation stringFromFile, size_t* sizeCommands, int** command );

#endif
