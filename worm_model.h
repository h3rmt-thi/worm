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

extern enum ResCodes initializeWorm(int headpos_y, int headpos_x, enum WormHeading dir, enum ColorPairs color, int len_max);
extern void showWorm(bool dead);
extern void moveWorm(enum GameStates *agame_state);
extern bool isInUseByWorm(int new_headpos_y, int new_headpos_x);
extern void setWormHeading(enum WormHeading dir);
extern void cleanWormTail();


#endif  // #define _WORM_MODEL_H
