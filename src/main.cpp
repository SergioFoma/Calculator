#include <TXLib.h>
#include <stdio.h>

#include "stack.h"
#include "checkError.h"
#include "paint.h"
#include "calculator.h"

int main(){
    size_t startSize = 8;
    stack_t stk = {};

    stackInit( &stk, startSize );
    
    calculator( &stk );
    
    stackDestroy( &stk );

    return 0;
}