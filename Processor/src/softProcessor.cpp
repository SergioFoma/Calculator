#include <string.h>

#include "softProcessor.h"
#include "paint.h"

void softProcessor( const char* nameOfByteFile, Code* informationOfCommand ){
    FILE* byteFile = fopen( nameOfByteFile, "r" );

    bufferInformation bufferFromFile = {};
    strInformation stringFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, byteFile );
    if( bufferError != correct ){
        colorPrintf(NOMODE, RED, "Error of init buffer struct :%s %s %d\n", __FILE__, __func__, __LINE__ );
        fclose( byteFile );
        return ;
    }
    errorCode stringError = initStringInformation( &stringFromFile );
    if( stringError != correct ){
        colorPrintf(NOMODE, RED, "Error of init string struct :%s %s %d\n", __FILE__, __func__, __LINE__ );
        fclose( byteFile );
        return ;
    }

    if ( !splitToLines( &bufferFromFile, &stringFromFile, byteFile ) ){
        colorPrintf( NOMODE, RED, "Error of spit lines :%s %s %d\n", __FILE__, __func__, __LINE__ );
        fclose( byteFile );
        return ;
    }

    getArrayWithCommand( stringFromFile, &(informationOfCommand->sizeOfCommands), &(informationOfCommand->command) );
    fclose( byteFile );
}


void getArrayWithCommand( strInformation stringFromFile, size_t* sizeCommands, int** arrayWithCommand ){

    size_t indexOfStrArray = 0, startSize = 8;
    *arrayWithCommand = (int*)calloc( startSize, sizeof( int ) );

    for( ; indexOfStrArray < stringFromFile.arraySize; indexOfStrArray++ ){
        char* lineForParsing = *(stringFromFile.arrayOfStr + indexOfStrArray);
        printf("%s\n", lineForParsing );
        if( (*sizeCommands) == startSize - 1 ){
            startSize *= 2;
            *arrayWithCommand = (int*)realloc( *arrayWithCommand, startSize * sizeof( int ) );
        }

        char* indexProbel = strchr( lineForParsing, ' ' );
        if ( indexProbel != NULL ){
            (*indexProbel) = '\0';
            int code = atoi( lineForParsing );
            int number = atoi( indexProbel + 1 );
            (*arrayWithCommand)[ (*sizeCommands)++ ] = code;
            (*arrayWithCommand)[ (*sizeCommands)++ ] = number;
        }
        else{
            int code = atoi( lineForParsing );
            (*arrayWithCommand)[ (*sizeCommands)++ ] = code;
        }
    }
}

