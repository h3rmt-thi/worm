#include "worm_model.h"

#include "board_model.h"
#include "worm.h"

// Initialize the worms
enum ResCodes initializeWorm(struct Worm *worm, const struct Pos head,
                             const enum WormHeading dir,
                             const enum ColorPairs color, const int len_max,
                             const int len_cur) {
    worm->maxindex = len_max - 1;
    worm->cur_lastindex = len_cur - 1;
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
void showWorm(struct Board *board, const struct Worm *worm) {
    int head = worm->headindex;
    int tail = (worm->headindex + 1) % (worm->cur_lastindex + 1);
    int i = worm->headindex;
    do {
        if (i == head)
            placeItem(board, worm->wormpos[i].y, worm->wormpos[i].x,
                      BC_USED_BY_WORM, SYMBOL_WORM_HEAD_ELEMENT, worm->wcolor);
        else if (i == tail)
            placeItem(board, worm->wormpos[i].y, worm->wormpos[i].x,
                      BC_USED_BY_WORM, SYMBOL_WORM_TAIL_ELEMENT, worm->wcolor);
        else
            placeItem(board, worm->wormpos[i].y, worm->wormpos[i].x,
                      BC_USED_BY_WORM, SYMBOL_WORM_INNER_ELEMENT, worm->wcolor);

        i = (i + worm->cur_lastindex) % (worm->cur_lastindex + 1);
    } while (i != worm->headindex && worm->wormpos[i].y != UNUSED_POS_ELEM);
}

int getWormLength(const struct Worm *worm) { return worm->cur_lastindex + 1; }

void cleanWormTail(struct Board *board, const struct Worm *worm) {
    // Compute tailindex
    const int tailindex = (worm->headindex + 1) % (worm->cur_lastindex + 1);
    // Check the array of worm elements.
    // Is the array element at tailindex already in use?
    // Checking either array theworm_wormpos_y
    // or theworm_wormpos_x is enough.
    if (worm->wormpos[tailindex].x != UNUSED_POS_ELEM) {
        // YES: place a SYMBOL_FREE_CELL at the tail's position
        placeItem(board, worm->wormpos[tailindex].y, worm->wormpos[tailindex].x,
                  BC_FREE_CELL, SYMBOL_FREE_CELL, COLP_FREE_CELL);
    }
}

void moveWorm(struct Board *board, struct Worm *worm,
              enum GameStates *game_state) {
    const struct Pos new_head = {worm->wormpos[worm->headindex].y + worm->dy,
                                 worm->wormpos[worm->headindex].x + worm->dx};

    // Check if we would leave the display if we move the worm's head according
    // to worm's last direction.
    // We are not allowed to leave the display's window.
    if (new_head.x < 0) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else if (new_head.x > getLastColOnBoard(board)) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else if (new_head.y < 0) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else if (new_head.y > getLastRowOnBoard(board)) {
        *game_state = WORM_OUT_OF_BOUNDS;
    } else {
        switch (getContentAt(board, new_head)) {
        case BC_FOOD_1:
            *game_state = WORM_GAME_ONGOING;
            // Grow worm according to food item digested
            growWorm(worm, BONUS_1);
            decrementNumberOfFoodItems(board);
            break;
        case BC_FOOD_2:
            *game_state = WORM_GAME_ONGOING;
            // Grow worm according to food item digested
            growWorm(worm, BONUS_2);
            decrementNumberOfFoodItems(board);
            break;
        case BC_FOOD_3:
            *game_state = WORM_GAME_ONGOING;
            // Grow worm according to food item digested
            growWorm(worm, BONUS_3);
            decrementNumberOfFoodItems(board);
            break;
        case BC_BARRIER:
            // That's bad: stop game
            *game_state = WORM_CRASH;
            break;
        case BC_USED_BY_WORM:
            // That's bad: stop game
            *game_state = WORM_CROSSING;
            break;
        default:;
        }
    }
    // Check the status of *game_state
    // Go on if nothing bad happened
    if (*game_state == WORM_GAME_ONGOING) {
        // So all is well: we did not hit anything bad and did not leave the
        // window. --> Update the worm structure.
        // Increment headindex
        // Go round if end of worm is reached (ring buffer)
        worm->headindex++;
        if (worm->headindex > worm->cur_lastindex) {
            worm->headindex = 0;
        }
        // Store new coordinates of head element in worm structure
        worm->wormpos[worm->headindex] = new_head;
    }
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

void growWorm(struct Worm *worm, enum Boni growth) {
    if (worm->cur_lastindex + growth <= worm->maxindex) {
        worm->cur_lastindex += growth;
    } else {
        worm->cur_lastindex = worm->maxindex;
    }
}