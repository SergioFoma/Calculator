#include "paint.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char* getColorString( Colors color ) {
    switch (color) {
        case PURPLE:
            return colorInPurple;
            break;
        case YELLOW:
            return colorInYellow;
            break;
        case GREEN:
            return colorInGreen;
            break;
        case BLUE:
            return colorInBlue;
            break;
        case RED:
            return colorInRed;
            break;
        default:
            return "";
            break;
    }
}

const char* getStyleString( Styles style ) {
    switch( style ) {
        case BOLD:
            return makeBold;
            break;
        case FADED:
            return makeFaded;
            break;
        case ITALICS:
            return makeItalics;
            break;
        case UNDERLINED:
            return makeUnderlined;
            break;
        case BLINKING:
            return makeBlinking;
            break;
        case CROSSEDOUT:
            return makeCrossedOut;
            break;
        case NOMODE:
            return "\033[";
            break;
        default:
            return "\033[";
            break;
    }
}

void colorPrintf( Styles style, Colors color,  const char* line ... ) {

    va_list args; // объявляем указатель
    va_start( args, line ); // связываем args с первым необязательным параметром. va_list как указатель

    printf("%s", getStyleString( style ) );
    printf("%s", getColorString(color));
    vprintf(line, args);
    printf("%s", colorReset );

    va_end( args );
}

void squareHelp() {

    colorPrintf(NOMODE, YELLOW, "    NOTE: this program solves a quadratic equation\n"
           "          of the form ax^2+bx+c\n\n"
           "   PRINT: the roots of the quadratic equation and their number\n\n"
           "   --help     display this help and exit\n\n"
           "   Some flags, that are used in int this program:\n\n"
           "   --Test      Prints unit tests that check the operation of a program \n"
           "               that solves a quadratic equation\n\n"
           "   --file-test Runs unit tests from a file\n"
           "     that will be specified after the flag\n\n"
        );

    printf("%s", colorReset );

}
