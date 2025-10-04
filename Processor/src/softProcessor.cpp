#include <string.h>

#include "softProcessor.h"
#include "checkError.h"
#include "paint.h"

void processorInit( Processor* SPU ){
    size_t startSizeForStack = 8;
    stackInit( &(SPU->stk), startSizeForStack );

    SPU->instructionPointer = 0;
    SPU->indexForRegister = RAX;
    SPU->spuErr = CORRECT_SPU;
    (SPU->code).sizeOfCommands = 0;
}

void softProcessor( const char* nameOfByteFile, Processor* SPU ){
    SPU_OK( SPU );

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

    getArrayWithCommand( stringFromFile, SPU );
    fclose( byteFile );
}


void getArrayWithCommand( strInformation stringFromFile, Processor* SPU ){
    SPU_OK( SPU );

    int** arrayWithCommand = &( (SPU->code).command );
    size_t* sizeCommands = &( (SPU->code).sizeOfCommands );

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

void processorDestroy( Processor* SPU ){
    stackDestroy( &(SPU->stk) );

    free( (SPU->code).command );

    (SPU->code).command = NULL;
    SPU->instructionPointer = 0;
    SPU->indexForRegister = RAX;
    SPU->spuErr = SPU_DESTROY;
}

void regsPrint( Processor* SPU ){
    SPU_OK( SPU );

    colorPrintf(NOMODE, YELLOW, "regs:\n");
    for( size_t index = 0; index < sizeOfRegs; index++ ){
        colorPrintf(NOMODE, YELLOW, "[ %lu ] = %d ", index, (SPU->regs)[index] );
    }
    printf("\n");
}
