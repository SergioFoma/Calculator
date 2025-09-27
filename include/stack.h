#ifndef H_STACK
#define H_STACK

#include <stdlib.h>
#include <stdbool.h>

typedef int type;

const size_t poison_ = 765911;
const type canary = 179171341; // 0xAADF00D

enum stackErr_t  {
    CORRECT                = 0,
    FILE_NULL_PTR          = 1,
    FUNC_NUL_PTR           = 2,
    DATA_NULL_PTR          = 3,
    NEGATIV_SIZE           = 4,
    NEGATIVE_COMPACITY     = 5,
    NEGATIVE_LINE          = 6,
};

struct stack_t{
    type* data;
    size_t size;
    size_t compacity;
    stackErr_t error;
};

void stackInit( stack_t *stk, size_t startSize);

void stackPush( stack_t *stk, type value );

type stackPop( stack_t *stk );

void stackPrint( stack_t *stk );

void stackDestroy( stack_t *size );

void calculateTest( stack_t *size );

#endif