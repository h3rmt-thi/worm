#include "board_model.h"

#include "messages.h"
#include "worm.h"
#include <curses.h>

int getLastRowOnBoard(const struct Board *board) { return board->last_row; }

int getLastColOnBoard(const struct Board *board) { return board->last_col; }

int getNumberOfFoodItems(const struct Board *board) {
    return board->food_items;
}

enum BoardCodes getContentAt(const struct Board *board,
                             const struct Pos position) {
    return board->cells[position.y][position.x];
}

void setNumberOfFoodItems(struct Board *board, const int n) {
    board->food_items = n;
}

void decrementNumberOfFoodItems(struct Board *board) { board->food_items -= 1; }

enum ResCodes initializeBoard(struct Board *board) {
    // Check dimensions of the board
    if (COLS < MIN_NUMBER_OF_COLS ||
        LINES < MIN_NUMBER_OF_ROWS + MA_ROWS_RESERVED) {
        char buf[100];
        sprintf(buf, "Das Fenster ist zu klein: wir brauchen %dx%d",
                MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS + MA_ROWS_RESERVED);
        showDialog(buf, "Bitte eine Taste druecken");
        return RES_FAILED;
    }
    // Maximal index of a row
    board->last_row = MIN_NUMBER_OF_ROWS - 1;
    // Maximal index of a column
    board->last_col = MIN_NUMBER_OF_COLS - 1;
    return RES_OK;
}

void placeItem(struct Board *board, const int y, const int x,
               const enum BoardCodes code, const char *symbol,
               const enum ColorPairs color_pair) {
    move(y, x);                      // Move cursor to (y,x)
    attron(COLOR_PAIR(color_pair));  // Start writing in selected color
    addstr(symbol);                  // Store symbol on the virtual display
    attroff(COLOR_PAIR(color_pair)); // Stop writing in selected color
    board->cells[y][x] = code;
}

enum ResCodes initializeLevel(struct Board *board) {
    // define local variables for loops etc
    // Fill board and screen buffer with empty cells.
    for (int y = 0; y <= board->last_row; y++) {
        for (int x = 0; x <= board->last_col; x++) {
            placeItem(board, y, x, BC_FREE_CELL, SYMBOL_FREE_CELL,
                      COLP_FREE_CELL);
        }
    }
    // Draw a line in order to separate the message area
    // Note: we cannot use function placeItem() since the message area
    // is outside the board!
    int y = board->last_row + 1;
    for (int x = 0; x < board->last_col; x++) {
        move(y, x);
        attron(COLOR_PAIR(COLP_BARRIER));
        addstr(SYMBOL_BARRIER);
        attroff(COLOR_PAIR(COLP_BARRIER));
    }
    // Draw a line to signal the rightmost column of the board.
    for (y = 0; y <= board->last_row; y++) {
        placeItem(board, y, board->last_col, BC_BARRIER, SYMBOL_BARRIER,
                  COLP_BARRIER);
    }
    // Barriers: use a loop
    for (y = 0; y <= 13; y++) {
        placeItem(board, y + 9, board->last_col / 3, BC_BARRIER, SYMBOL_BARRIER,
                  COLP_BARRIER);
    }
    for (y = 0; y <= 13; y++) {
        placeItem(board, y + 3, board->last_col / 3 * 2, BC_BARRIER,
                  SYMBOL_BARRIER, COLP_BARRIER);
    }
    // Food
    placeItem(board, 3, 3, BC_FOOD_1, SYMBOL_FOOD_1, COLP_FOOD_1);
    placeItem(board, 6, 6, BC_FOOD_1, SYMBOL_FOOD_1, COLP_FOOD_1);
    placeItem(board, 16, 5, BC_FOOD_1, SYMBOL_FOOD_2, COLP_FOOD_2);
    placeItem(board, 22, 44, BC_FOOD_1, SYMBOL_FOOD_2, COLP_FOOD_2);
    placeItem(board, 4, 13, BC_FOOD_1, SYMBOL_FOOD_2, COLP_FOOD_2);
    placeItem(board, 19, 9, BC_FOOD_1, SYMBOL_FOOD_2, COLP_FOOD_2);
    placeItem(board, 17, 16, BC_FOOD_1, SYMBOL_FOOD_3, COLP_FOOD_3);
    placeItem(board, 22, 33, BC_FOOD_1, SYMBOL_FOOD_3, COLP_FOOD_3);
    placeItem(board, 15, 36, BC_FOOD_1, SYMBOL_FOOD_3, COLP_FOOD_3);
    placeItem(board, 7, 56, BC_FOOD_1, SYMBOL_FOOD_3, COLP_FOOD_3);

    // Initialize number of food items
    // Attention: must match number of items placed on the board above
    board->food_items = 10;
    return RES_OK;
}
