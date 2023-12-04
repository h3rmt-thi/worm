#ifndef _WORM_H
#define _WORM_H

enum ResCodes {
    RES_OK,
    RES_FAILED,
};

enum ColorPairs {
    COLP_FREE_CELL = 0,
    COLP_DATA,
    COLP_USER_WORM,
};

// Dimensions and bounds
#define NAP_TIME    100   // Time in milliseconds to sleep between updates of display
#define MIN_NUMBER_OF_ROWS  3   // The guaranteed number of rows available for the board
#define MIN_NUMBER_OF_COLS 10   // The guaranteed number of columns available for the board
#define WORM_LENGTH 30 // Maximal length of the worm

// Numbers for color pairs used by curses macro COLOR_PAIR
//#define COLP_USER_WORM 1

// Symbols to display
#define SYMBOL_WORM_INNER_ELEMENT '0'
#define SYMBOL_FREE_CELL ' '

// ### Codes for the array of positions ###
// Unused element in the worm arrays of positions
#define UNUSED_POS_ELEM -1

enum GameStates {
    WORM_GAME_ONGOING,
    WORM_OUT_OF_BOUNDS,
    WORM_CROSSING, // Worm head crossed another worm element
    WORM_GAME_QUIT,
};

#endif  // #define _WORM_H
