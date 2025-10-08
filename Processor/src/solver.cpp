#include "solver.h"
#include "paint.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

bool isEqual( double firstNumber, double secondNumber ) {
    return (  fabs( firstNumber - secondNumber ) <= delta );
}

void solveEquation( Coefficients coefficients, SolveResult* answer ) {

    assert( answer != NULL );

    if ( isEqual( coefficients.a, 0.0 ) )  {
        solveLinear( coefficients, answer );
    }
    else {
        solveSquare( coefficients, answer );
    }
}

void solveSquare( Coefficients coefficients, SolveResult* answer ) {

    assert( answer != NULL );

    double D = coefficients.b * coefficients.b - 4 * coefficients.a * coefficients.c;

    if ( D < 0 ) {
        ( answer->countRoots ) = zeroRoot;
    }
    else {
        double firstPart = - coefficients.b / ( 2 * coefficients.a );
        double secondPart = sqrt( D ) / ( 2 * coefficients.a );

        if ( secondPart < 0 ) {
            ( answer->x1) = firstPart + secondPart;
            ( answer->x2) = firstPart - secondPart;
        }
        else {
            ( answer->x1) = firstPart - secondPart;
            ( answer->x2) = firstPart + secondPart;
        }
        ( answer->countRoots) = ( isEqual(answer->x1, answer->x2) ) ? twoSameRoot: twoRoot;
    }
}

void solveLinear( Coefficients coefficients, SolveResult* answer ) {

    assert( answer != NULL );

    if( isEqual( coefficients.b, 0 ) && isEqual( coefficients.c, 0) ) {
        ( answer->countRoots ) = alotRoot;
    }
    else if( isEqual( coefficients.b, 0 ) && !isEqual( coefficients.c, 0 ) ){
        ( answer->countRoots) = zeroRoot;
    }
    else {
        answer->x1 = answer->x2 = -coefficients.c / coefficients.b;
        ( answer->countRoots ) = oneRoot;
    }
}

void printResult( Coefficients coefficients, SolveResult answer ) {

    colorPrintf( NOMODE, BLUE, "\n\na: %lg b: %lg c: %lg\n", coefficients.a, coefficients.b, coefficients.c );

    switch ( answer.countRoots ) {
        case zeroRoot:
            colorPrintf( NOMODE, RED, "Нет корней.\n");
            break;

        case twoRoot:
            colorPrintf( NOMODE, GREEN, "\nУравнение квадратное.\nМеньший корень: %lg\nБольший корень: %lg\n",
            answer.x1, answer.x2 );
            break;

        case oneRoot:
            colorPrintf( NOMODE, GREEN, "\nУравнение линейное.\nКорень уравнения: %lg\n", answer.x1 );
            break;

        case alotRoot:
            colorPrintf( NOMODE, GREEN, "\nУравнение имеет бесконечное количество корней.\n");
            break;

        case twoSameRoot:
            colorPrintf( NOMODE, GREEN,"\nУравнение квадратное.\nИмеет единственный корень: %lg\n", answer.x1 );
            break;

        default:
            colorPrintf( UNDERLINED, RED, "\nОшибка. Не определено количество корней.\n");
            break;
    }
}

