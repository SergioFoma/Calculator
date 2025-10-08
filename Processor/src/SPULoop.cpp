#include <stdio.h>
#include <string.h>
#include <cmath>

#include "SPULoop.h"
#include "softProcessor.h"
#include "paint.h"
#include "checkError.h"
#include "solver.h"

void calculationFromProcessor( Processor *SPU, const char* byteFile ){
    SPU_OK( SPU );
    
    softProcessor( byteFile, SPU );
    size_t index = 0;
    int first = 0, last = 0, indexOfRegister = 0, DO = 1;

    while( index < (SPU->code).sizeOfCommands && DO ){
        SPU->instructionPointer = index;
        switch( (SPU->code).command[ index ] ){
            case HLT:
                DO = 0;
                break;
            case PUSH:
                DO_PUSH;
                break;
            case MUL:
                DO_MUL;
                break;
            case SUB:
                DO_SUB;
                break;
            case OUT:
                DO_OUT;
                break;
            case ADD:
                DO_ADD;
                break;
            case DIV:
                DO_DIV;
                break;
            case SQRT:
                DO_SQRT;
                break;
            case IN:
                DO_IN;
                break;
            case SOLVER:{
                DO_SOLVE_EQUATION;
                break;
            }
            case POPR:
                DO_POPR;
                break;
            case PUSHR:
                DO_PUSHR;
                break;
            default:
                break;
        }
        ++index;
    }

}
