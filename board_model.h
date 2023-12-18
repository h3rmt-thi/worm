#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H

#include "worm.h"
#include <curses.h>

// Positions on the board
struct Pos {
	int y;
	int x;
};

void placeItem(struct Pos, chtype symbol, enum ColorPairs);
int getLastRow();
int getLastCol();

#endif // #define _BOARD_MODEL_H
