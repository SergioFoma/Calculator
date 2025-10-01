//#include <TXLib.h>
#include <stdio.h>

#include "stack.h"
#include "checkError.h"
#include "paint.h"
#include "calculator.h"
#include "processor.h"

int main(){
    size_t startSize = 8;
    stack_t stk = {};

    ASM( "assembler.asm", "BYTE-CODE.txt" );
    return 0;

    stackInit( &stk, startSize );

    calculator( &stk ); // TODO SPULoop

    stackDestroy( &stk );

    return 0;
}
