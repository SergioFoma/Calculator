#include <string.h>
#include <stdio.h>

#include "softProcessor.h"
#include "checkError.h"
#include "paint.h"

void processorInit( Processor* SPU ){
    size_t startSizeForStack = startSize;
    stackInit( &(SPU->stk), startSizeForStack );
    stackInit( &(SPU->regAddr), startSizeForStack );

    SPU->RAM = (int*)calloc( sizeRam, sizeof( int ) );
    SPU->instructionPointer = 0;
    SPU->indexForRegister = RAX;
    SPU->spuErr = CORRECT_SPU;
    (SPU->code).sizeOfCommands = 0;
}

void softProcessor( const char* nameOfByteFile, Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    FILE* byteFile = fopen( nameOfByteFile, "r" );

    bufferInformation bufferFromFile = {};

    errorCode bufferError = initBufferInformation( &bufferFromFile, byteFile );
    if( bufferError != correct ){
        colorPrintf(NOMODE, RED, "Error of init buffer struct :%s %s %d\n", __FILE__, __func__, __LINE__ );
        fclose( byteFile );
        return ;
    }

    getArrayWithCommand( bufferFromFile, SPU );
    fclose( byteFile );
}


void getArrayWithCommand( bufferInformation bufferFromFile, Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    int** arrayWithCommand = &( (SPU->code).command );
    size_t* sizeCommands = &( (SPU->code).sizeOfCommands );

    size_t  indexOfBuffer = 0, startSize = 8;
    int countOfReading = 0;
    *arrayWithCommand = (int*)calloc( startSize, sizeof( int ) );

    while ( indexOfBuffer < bufferFromFile.bufferSize ) {
        if( (*sizeCommands) == startSize - 1 ){
            startSize *= 2;
            *arrayWithCommand = (int*)realloc( *arrayWithCommand, startSize * sizeof( int ) );
        }

        sscanf( bufferFromFile.buffer + indexOfBuffer, "%d%n", (*arrayWithCommand + (*sizeCommands)++ ), &countOfReading );
        if( countOfReading == 0){
            break;
        }
        indexOfBuffer += countOfReading;
    }
}

void processorDestroy( Processor* SPU ){
    stackDestroy( &(SPU->stk) );
    stackDestroy( &(SPU->regAddr) );
    free( (SPU->code).command );
    free( SPU->RAM );

    (SPU->code).command = NULL;
    SPU->instructionPointer = 0;
    SPU->indexForRegister = RAX;
    SPU->spuErr = SPU_DESTROY;
}

void regsPrint( Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    colorPrintf(NOMODE, YELLOW, "regs:\n");
    for( size_t index = 0; index < sizeOfRegs; index++ ){
        colorPrintf(NOMODE, YELLOW, "[ %lu ] = %d ", index, (SPU->regs)[index] );
    }
    printf("\n");
}

void ramPrint( Processor* SPU ){
    SPU_OK( SPU, (void)0 );

    colorPrintf( NOMODE, BLUE, "RAM:\n" );
    for( size_t index = 0; index < sizeOfVideoMemory; index++){
        printf("%c ", (SPU->RAM)[ index ] );
        if( (index+1) % countOfSymbolInLine == 0 ){
            printf("\n");
        }
    }
    printf("\n");
}

void doColor( Processor* SPU, size_t index ){
    switch( (SPU->RAM)[ index ] ){
        case PURPLE_COLOR:
            colorPrintf( NOMODE, PURPLE, "%c ", (SPU->RAM)[ index + 1 ] );
            break;
        case RED_COLOR:
            colorPrintf( NOMODE, RED, "%c ", (SPU->RAM)[ index + 1 ] );
            break;
        default:
            printf( "%c ", (SPU->RAM)[ index + 1 ] );
            break;
    }
}
