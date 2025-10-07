#include "stack.h"
#include "softProcessor.h"

#ifdef UNDEBUG
#define STACK_OK( err ) do {} while(false)
#define SPU_OK( SPU ) do {} while(false)
#else

void stackDump( const char* file, const char* func, int line, stack_t *stk );
stackErr_t stackVerify( stack_t *stk );

void processorDump( const char* file, const char* func, int line, Processor *SPU );
processorError processorVerify( Processor* SPU );

#define STACK_OK( stk )                                              \
    do{                                                              \
        if( stackVerify( stk ) != CORRECT ) {                        \
            ( stackDump( __FILE__, __func__, __LINE__,  stk ) );     \
            return ;                                                 \
        }                                                            \
    }while( false )


#define SPU_OK( SPU )                                                                               \
    do{                                                                                             \
        if( processorVerify( SPU ) != CORRECT_SPU ){                                                \
            ( processorDump( __FILE__, __func__, __LINE__, SPU ) );                                 \
            return ;                                                                                \
        }                                                                                           \
    }while( false )


#endif
