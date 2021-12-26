#ifndef SIFE_CONSOLE_H
#define SIFE_CONSOLE_H

    #define ESCAPE "\033"

    // COLORS
    #define COLOR_RESET ESCAPE "[0m"
    #define COLOR_RED ESCAPE "[0;31m"
    #define COLOR_BRED ESCAPE "[1;31m"
    #define COLOR_GREEN ESCAPE "[0;32m"
    #define COLOR_BGREEN ESCAPE "1;32m"
    #define COLOR_YELLOW ESCAPE "[0;33m"
    #define COLOR_BYELLOW ESCAPE "[1;33m"
    #define COLOR_BLUE ESCAPE "[0;34m"
    #define COLOR_BBLUE ESCAPE "[1;34m"
    #define COLOR_MAGNETA ESCAPE "[0;35m"
    #define COLOR_BMAGENTA ESCAPE "[1;35m"
    #define COLOR_CYAN ESCAPE "[0;36m"
    #define COLOR_BCYAN ESCAPE "[1;36m"

    // CLEARS
    #define CLEAR_LINE ESCAPE "[2K\r"
    #define CLEAR_SCREEN ESCAPE "[2J\r"

    // CURSOR
    #define MOVE_UP ESCAPE "[1A\r"
    #define MOVE_DOWN ESCAPE "[1B\r"
    #define MOVE_RIGHT ESCAPE "[1C"
    #define MOVE_LEFT ESCAPE "[1D"

#endif
