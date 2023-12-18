#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H

#include "worm.h"
#include <curses.h>

// Positions on the board
struct Pos {
    int y;
    int x;
};

// Codes on the board
enum BoardCodes {
    BC_FREE_CELL,    // Cell is free
    BC_USED_BY_WORM, // Cell occupied by worm
    BC_FOOD_1,       // Food type 1; if hit by worm -> bonus of type 1
    BC_FOOD_2,       // Food type 2; if hit by worm -> bonus of type 2
    BC_FOOD_3,       // Food type 3; if hit by worm -> bonus of type 3
    BC_BARRIER       // A barrier; if hit by worm -> game over
};

// Board
// A board structure
struct Board {
    int last_row; // Last usable row on the board
    int last_col; // Last usable column on the board

    enum BoardCodes cells[MIN_NUMBER_OF_ROWS][MIN_NUMBER_OF_COLS];
    // A 2-dimensional array for storing the contents of the board.
    //
    // Since the worm is not permitted to cross over itsself
    // nor other elements (apart from food) we do not need a reference
    // counter for occupied cells.

    int food_items; // Number of food items left in the current level
};

// ### Codes for the array of positions ###
// Unused element in the worm arrays of positions
#define UNUSED_POS_ELEM (-1)
#define WORM_LENGTH (MIN_NUMBER_OF_ROWS * MIN_NUMBER_OF_COLS)
#define WORM_INITIAL_LENGTH 6

enum ResCodes initializeBoard(struct Board *);
void placeItem(struct Board *, int y, int x, enum BoardCodes,
               const char *constsymbol,
               enum ColorPairs);
enum ResCodes initializeLevel(struct Board *);

// Getters
int getNumberOfFoodItems(const struct Board *);
enum BoardCodes getContentAt(const struct Board *, struct Pos);
int getLastRowOnBoard(const struct Board *);
int getLastColOnBoard(const struct Board *);

// Setters
void decrementNumberOfFoodItems(struct Board *);
void setNumberOfFoodItems(struct Board *, int n);

#endif // #define _BOARD_MODEL_H
