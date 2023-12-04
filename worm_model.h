#ifndef _WORM_MODEL_H
#define _WORM_MODEL_H

#include <stdbool.h>
#include "worm.h"

enum WormHeading {
    WORM_UP,
    WORM_DOWN,
    WORM_LEFT,
    WORM_RIGHT,
};

enum ResCodes initializeWorm(int headpos_y, int headpos_x, enum WormHeading dir, enum ColorPairs color, int len_max);
void showWorm(bool dead);
void moveWorm(enum GameStates *agame_state);
bool isInUseByWorm(int new_headpos_y, int new_headpos_x);
void setWormHeading(enum WormHeading dir);
void cleanWormTail();


#endif  // #define _WORM_MODEL_H
