#include "worm_model.h"

#include "board_model.h"
#include "worm.h"
#include <curses.h>

// Initialize the worms
enum ResCodes initializeWorm(struct Worm *worm, const struct Pos head,
                             const enum WormHeading dir,
                             const enum ColorPairs color, const int len_max) {
    worm->maxindex = len_max - 1;
    worm->headindex = 0;

    for (int i = 0; i < worm->maxindex; i++) {
        worm->wormpos[i].x = UNUSED_POS_ELEM;
        worm->wormpos[i].y = UNUSED_POS_ELEM;
    }

    worm->wormpos[worm->headindex].x = head.x;
    worm->wormpos[worm->headindex].y = head.y;

    // Initialize the heading of the worm
    setWormHeading(worm, dir);

    // Initialze color of the worm
    worm->wcolor = color;
    return RES_OK;
}

// Show the worms's elements on the display
// Simple version
void showWorm(const struct Worm *worm, const bool dead) {
    // Due to our encoding we just need to show the head element
    // All other elements are already displayed
    if (dead)
        placeItem(worm->wormpos[worm->headindex], SYMBOL_WORM_INNER_ELEMENT,
                  COLP_DATA);
    else
        placeItem(worm->wormpos[worm->headindex], SYMBOL_WORM_INNER_ELEMENT,
                  COLP_USER_WORM);
}

void cleanWormTail(const struct Worm *worm) {
    // Compute tailindex
    const int tailindex = (worm->headindex + 1) % worm->maxindex;
    // Check the array of worm elements.
    // Is the array element at tailindex already in use?
    // Checking either array theworm_wormpos_y
    // or theworm_wormpos_x is enough.
    if (worm->wormpos[tailindex].x != UNUSED_POS_ELEM) {
        // YES: place a SYMBOL_FREE_CELL at the tail's position
        placeItem(worm->wormpos[tailindex], SYMBOL_FREE_CELL, COLP_FREE_CELL);
    }
}

void moveWorm(struct Worm *worm, enum GameStates *game_state) {
    const struct Pos new_head = {worm->wormpos[worm->headindex].y + worm->dy,
                                 worm->wormpos[worm->headindex].x + worm->dx};

    attron(COLOR_PAIR(COLP_FREE_CELL));
    mvprintw(getLastRow(), 40, "         ");
    attroff(COLOR_PAIR(COLP_FREE_CELL));
    attron(COLOR_PAIR(COLP_DATA));
    mvprintw(getLastRow(), 40, "%dx%d %d", new_head.y, new_head.x,
             worm->headindex);
    attroff(COLOR_PAIR(COLP_DATA));

    // Check if we would leave the display if we move the worm's head according
    // to worm's last direction.
    // We are not allowed to leave the display's window.
    if (new_head.x < 0) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else if (new_head.x > getLastCol()) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else if (new_head.y < 0) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else if (new_head.y > getLastRow()) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else {
        // We will stay within bounds.
        if (isInUseByWorm(worm, new_head)) {
            // That's bad: stop game
            *game_state = WORM_CROSSING;
        }
    }
    // Check the status of *agame_state
    // Go on if nothing bad happened
    if (*game_state == WORM_GAME_ONGOING) {
        // So all is well: we did not hit anything bad and did not leave the
        // window. --> Update the worm structure.
        // Increment theworm_headindex
        // Go round if end of worm is reached (ring buffer)
        worm->headindex = (worm->headindex + 1) % worm->maxindex;
        // Store new coordinates of head element in worm structure
        worm->wormpos[worm->headindex] = new_head;
    }
}

// A simple collision detection
bool isInUseByWorm(const struct Worm *worm, const struct Pos new_head) {
    // bool collision = false;

    for (int i = 0; i <= worm->maxindex; i++) {
        // if theworm_wormpos_x[i] == UNUSED_POS_ELEM theworm_wormpos_y[i] ==
        // UNUSED_POS_ELEM is also true
        if (worm->wormpos[i].y == UNUSED_POS_ELEM) {
            // break;
            return false;
        }

        if (worm->wormpos[i].x == new_head.x &&
            worm->wormpos[i].y == new_head.y) {
            // collision = true;
            // break;
            return true;
        }
    }

    // Return what we found out.
    // return collision;
    return false;
}

// Setters
void setWormHeading(struct Worm *worm, const enum WormHeading dir) {
    switch (dir) {
    case WORM_UP: // User wants up
        worm->dx = 0;
        worm->dy = -1;
        break;
    case WORM_DOWN: // User wants down
        worm->dx = 0;
        worm->dy = 1;
        break;
    case WORM_LEFT: // User wants left
        worm->dx = -1;
        worm->dy = 0;
        break;
    case WORM_RIGHT: // User wants right
        worm->dx = 1;
        worm->dy = 0;
        break;
    }
}

struct Pos getWormHeadPos(const struct Worm *worm) {
    // Structures are passed by value!
    // -> we return a copy here
    return worm->wormpos[worm->headindex];
}
