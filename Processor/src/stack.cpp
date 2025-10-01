#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "checkError.h"
#include "paint.h"

void stackInit( stack_t *stk, size_t startSize ){
    (stk->data) = (type*)calloc( startSize + 2, sizeof( type ) );

    if( stk->data == NULL ){
        stk->error = DATA_NULL_PTR;
        return ;
    }

    stk->size = 1;
    stk->compacity = startSize + 2;
    (stk->data)[0] = canary;
    (stk->data)[stk->compacity - 1] = canary;
    stk->error = CORRECT;

    return ;
}

void stackPush( stack_t *stk, type value ){
    STACK_OK( stk );

    if( stk->size == stk->compacity - 1 ){
        (stk->compacity) *= 2;
        stk->data = (type*)realloc( stk->data, (stk->compacity) * sizeof( type ) );
        if( stk->data == NULL ){
            stk->error = DATA_NULL_PTR;
            return ;
        }
        stk->data[ stk->compacity - 1] = canary;
    }
    (stk->data)[stk->size++] = value;

    return ;
}

type stackPop( stack_t *stk ){
    if ( stackVerif( stk ) != CORRECT ){
        return poison_;
    }

    if( stk->size <= 1 || stk->compacity <= 1){
        return poison_;
    }
    return (stk->data)[--(stk->size)];
}

void stackPrint( stack_t *stk ){
    STACK_OK( stk );

    colorPrintf(NOMODE, GREEN, "\nstack: \n");
    for( size_t index = 0; index < (stk->size) - 1; index++ ){
        colorPrintf(NOMODE, GREEN, "[%u] = %d ", index, (stk->data)[index+1] );
    }
    printf("\n");

    return ;
}

void stackDestroy( stack_t *stk ){
    free( stk->data );
    stk->data = NULL;
    stk->compacity = -1;
    stk->size = -1;
    stk->error = CORRECT;

    return ;
}

