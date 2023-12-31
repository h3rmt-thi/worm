#ifndef WORM_H
#define WORM_H

enum ResCodes {
    RES_OK,
    RES_FAILED,
    RES_INTERNAL_ERROR,
};

enum ColorPairs {
    COLP_USER_WORM = 1,
    COLP_FREE_CELL,
    COLP_FOOD_1,
    COLP_FOOD_2,
    COLP_FOOD_3,
    COLP_BARRIER
};

// Dimensions and bounds
#define MA_ROWS_RESERVED 4
#define NAP_TIME 100 // Time in milliseconds to sleep between updates of display
#define MIN_NUMBER_OF_ROWS                                                     \
    30 // The guaranteed number of rows available for the board
#define MIN_NUMBER_OF_COLS                                                     \
    60 // The guaranteed number of columns available for the board

// Symbols to display
#define SYMBOL_FREE_CELL " "
#define SYMBOL_BARRIER "#"
#define SYMBOL_FOOD_1 "🍟"
#define SYMBOL_FOOD_2 "🍔"
#define SYMBOL_FOOD_3 "ඞ"
#define SYMBOL_WORM_HEAD_ELEMENT "0"
#define SYMBOL_WORM_INNER_ELEMENT "o"
#define SYMBOL_WORM_TAIL_ELEMENT "`"

// Game state codes
enum GameStates {
    WORM_GAME_ONGOING,
    WORM_CRASH,         // Crashed into a barrier
    WORM_OUT_OF_BOUNDS, // Left screen
    WORM_CROSSING,      // Worm head crossed another worm element
    WORM_GAME_QUIT,     // User likes to quit
};

#endif // #define WORM_H
