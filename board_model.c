#include "board_model.h"

#include "worm.h"
#include <curses.h>

// Place an item onto the curses display.
void placeItem(const struct Pos pos, const chtype symbol,
               const enum ColorPairs color_pair) {
    //  Store item on the display (symbol code)
    move(pos.y, pos.x);              // Move cursor to (y,x)
    attron(COLOR_PAIR(color_pair));  // Start writing in selected color
    addch(symbol);                   // Store symbol on the virtual display
    attroff(COLOR_PAIR(color_pair)); // Stop writing in selected color
}

// Get the last usable row on the display
int getLastRow() { return LINES - 1 - MA_ROWS_RESERVED; }

// Get the last usable column on the display
int getLastCol() { return COLS - 1; }
