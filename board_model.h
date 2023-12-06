#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H

#include "worm.h"
#include <curses.h>

extern void placeItem(int y, int x, chtype symbol, enum ColorPairs color_pair);
extern int getLastRow();
extern int getLastCol();

#endif // #define _BOARD_MODEL_H
