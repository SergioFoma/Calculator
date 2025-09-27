#include <stdio.h>
#include "stack.h"
#include "checkError.h"

const int poison = 765911;

void stackDump( const char* file, const char* func, int line, stack_t *stk ){

    FILE* fileForDump = fopen("StackStatus.txt", "a");

    if( fileForDump == NULL){
        printf("\nstackDump can`t to write information about stack in file\n");
        stk->error = FILE_NULL_PTR;
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
    fprintf( fileForDump, "stack[%p]\nSize = %zu\nCompacity = %zu\n.data[%p]\nCode of error = %d\n", 
            stk, stk->size, stk->compacity, stk->data, stk->error );
    for( size_t index = 0; index < ( stk->compacity ); index++ ){
        if( index < (stk->size) || index == stk->compacity - 1 ){
            fprintf( fileForDump, "*[%zu] = %d\n", index, (stk->data)[index] );
        }
        else{
            fprintf( fileForDump, " [%zu] = %d( POISON )\n", index, poison);
        }
    }
    fclose( fileForDump );

    return ;
}