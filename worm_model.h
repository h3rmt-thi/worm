#ifndef _WORM_MODEL_H
#define _WORM_MODEL_H

#include "board_model.h"
#include "worm.h"

#include <stdbool.h>

enum WormHeading {
    WORM_UP,
    WORM_DOWN,
    WORM_LEFT,
    WORM_RIGHT,
};

// Boni for eating food
enum Boni {
    BONUS_1 = 2, // additional length for worm when consuming food of type 1
    BONUS_2 = 4, // additional length for worm when consuming food of type 2
    BONUS_3 = 6, // additional length for worm when consuming food of type 3
};

// A worm structure
struct Worm {
    int cur_lastindex;
    int maxindex;
    // Last usable index into the array pointed to by wormpos
    int headindex; // An index into the array for the worm's head position
    // 0 <= headindex <= maxindex
    struct Pos wormpos[WORM_LENGTH]; // Array of x,y positions of all elements
    // of the worm
    // The current heading of the worm
    // These are offsets from the set {-1,0,+1}
    int dx;
    int dy;
    // Color of the worm
    enum ColorPairs wcolor;
};

enum ResCodes initializeWorm(struct Worm *, struct Pos head,
                             enum WormHeading dir, enum ColorPairs color,
                             int len_max, int len_cur);
void growWorm(struct Worm *, enum Boni growth);
void showWorm(struct Board *, const struct Worm *);
void moveWorm(struct Board *, struct Worm *, enum GameStates *);
void setWormHeading(struct Worm *, enum WormHeading dir);
void cleanWormTail(struct Board *, const struct Worm *);
struct Pos getWormHeadPos(const struct Worm *);
int getWormLength(const struct Worm *);

#endif // #define _WORM_MODEL_H
