#include <stdio.h>
#include "stack.h"
#include "softProcessor.h"
#include "checkError.h"

const int poison = 765911;

stackErr_t stackVerify( stack_t *stk ){
    if( stk == NULL ){
        stk->error = STK_NULL_PTR;
        return STK_NULL_PTR;
    }
    else if( stk->data == NULL ){
        stk->error = DATA_NULL_PTR;
        return DATA_NULL_PTR;
    }
    else if( stk->size < 1 ){
        stk->error = NEGATIVE_SIZE;
        return NEGATIVE_SIZE;
    }
    else if( stk->capacity < 1 ){
        stk->error = NEGATIVE_CAPACITY;
        return NEGATIVE_CAPACITY;
    }

    if( (stk->data)[0] != canary || (stk->data)[stk->capacity - 1] != canary){
        stk->error = STACK_OVERFLOW;
        return STACK_OVERFLOW;
    }

    return CORRECT;
}
void stackDump( const char* file, const char* func, int line, stack_t *stk ){

    FILE* fileForDump = fopen("StackStatus.txt", "a");

    if( fileForDump == NULL){
        printf("\nstackDump can`t to write information about stack in file\n");
        stk->error = FILE_NULL_PTR;
        return ;
    }
    else if( stk == NULL ){
        fprintf( fileForDump, "\nSTK is null ptr in function%s\n", func );
        stk->error = STK_NULL_PTR;
        return ;
    }
    else if( file == NULL ){
        fprintf( fileForDump, "\nName of file %s - NULL PTR in function %s, in line %d\n", file, func, line);
        fclose( fileForDump );
        stk->error = FILE_NULL_PTR;
        return ;
    }
    else if( func == NULL ){
        fprintf( fileForDump, "\nName of function %s - NULL PTR in file %s, in line %d\n",
                  func, file, line );
        fclose( fileForDump );
        stk->error = FUNC_NUL_PTR;
        return ;
    }
    else if ( line < 0 ){
        fprintf( fileForDump, "\nThe line %d is less than zero in file %s, in function %s\n",
                 line, file, func);
        fclose( fileForDump );
        stk->error = NEGATIVE_LINE;
        return ;
    }
    else if ( stk->data == NULL ){
        fprintf( fileForDump, "\nThe pointer to the zero date in file %s, in function %s, in line %d\n",
                file, func, line);
        fclose( fileForDump );
        stk->error = DATA_NULL_PTR;
        return ;
    }
    else if ( stk->error != CORRECT ){
        fprintf( fileForDump, "\nCode of error - 1 in file %s, in function %s, in line %d\n", file, func, line);
        fclose( fileForDump );
        return ;
    }
    fprintf( fileForDump, "\nstackDump called from file %s, in function %s, in line %d\n", file, func, line);
    fprintf( fileForDump, "stack[%p]\nSize = %lu\nCompacity = %lu\n.data[%p]\nCode of error = %d\n",
            stk, stk->size, stk->capacity, stk->data, stk->error );
    for( size_t index = 0; index < ( stk->capacity ); index++ ){
        if( index < (stk->size) || index == stk->capacity - 1 ){
            fprintf( fileForDump, "*[%lu] = %d\n", index, (stk->data)[index] );
        }
        else{
            fprintf( fileForDump, " [%lu] = %d( POISON )\n", index, poison);
        }
    }
    fclose( fileForDump );

    return ;
}

processorError processorVerify( Processor* SPU ){
    if( SPU == NULL ){
        return SPU_NULL_PTR;
    }
    if( stackVerify( &(SPU->stk) )  != CORRECT ){
        return STACK_ERROR;
    }

    if( SPU->spuErr != CORRECT_SPU ){
        return (SPU->spuErr );
    }

    return CORRECT_SPU;
}

void processorDump( const char* file, const char* func, int line, Processor *SPU ){

    FILE* fileForSPU = fopen( "ProcessorStatus.txt", "a" );

    if( stackVerify( &(SPU->stk) ) != CORRECT ){
        stackDump( __FILE__, __func__, __LINE__, &(SPU->stk) );
        SPU->spuErr = STACK_ERROR;
        fclose( fileForSPU );
        return ;
    }
    else if( stackVerify( &(SPU->regAddr) ) != CORRECT ){
        stackDump( __FILE__, __func__, __LINE__, &(SPU->regAddr) );
        SPU->spuErr = STACK_ERROR;
        fclose( fileForSPU );
        return ;
    }
    else if( (SPU->code).command == NULL ){
        fprintf( fileForSPU, "\nIn struct code: command - NULL PTR\n");
        SPU->spuErr = CODE_NULL_PTR;
        fclose( fileForSPU );
        return ;
    }
    else if( SPU->spuErr != CORRECT_SPU ){
        fprintf( fileForSPU, "\nIn SPU spuErr = %d\n", SPU->spuErr );
        fclose( fileForSPU );
        return ;
    }

    fprintf( fileForSPU, "\nprocessorDump called from file %s, in function %s, in line %d\n",
              file, func, line);
    fprintf( fileForSPU, "\nSPU[%p]\nCode.code[%p]\nregs[%p]\nspuErr = %d\n",
                          SPU, &((SPU->code).command), &(SPU->regs), SPU->spuErr );
    fprintf( fileForSPU, "\nstack[%p]\nSize = %lu\nCapacity = %lu\n.data[%p]\nCode of error = %d\n",
            &(SPU->stk), (SPU->stk).size, (SPU->stk).capacity, &((SPU->stk).data), (SPU->stk).error );

    size_t index = 0;

    fprintf( fileForSPU, "\nPrint element in stack: \n" );
    for( index = 0; index < ( (SPU->stk).capacity ); index++ ){
        if( index < ((SPU->stk).size) || index == (SPU->stk).capacity - 1 ){
            fprintf( fileForSPU, "*[%lu] = %d\n", index, ((SPU->stk).data)[index] );
        }
        else{
            fprintf( fileForSPU, " [%lu] = %d( POISON )\n", index, poison);
        }
    }

    fprintf( fileForSPU, "\nPrint element in code: \n" );
    for( index = 0; index < ( (SPU->code).sizeOfCommands ); index++ ){
        fprintf( fileForSPU, "[%lu] = %d ", index, ( (SPU->code).command )[index] );
    }

    fprintf( fileForSPU, "\nInstruction Pointer = %ld\n", SPU->instructionPointer );

    fprintf( fileForSPU, "\nPrint element in regs: \n" );
    for( index = 0; index < sizeOfRegs; index++ ){
        fprintf( fileForSPU, "[%lu] = %d ", index, (SPU->regs)[index] );
    }

    fprintf( fileForSPU, "\nIndex of last element in regs = %d\n", SPU->indexForRegister );

    fclose( fileForSPU );
}
