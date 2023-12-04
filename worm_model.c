#include "worm_model.h"

#include <curses.h>
#include "worm.h"
#include "board_model.h"

// Data defining the worm
// The current heading of the worm
// These are offsets from the set {-1,0,+1}
int theworm_dx;
int theworm_dy;

enum ColorPairs theworm_wcolor;

// Array of y positions for worm elements
int theworm_wormpos_y[WORM_LENGTH];
// Array of x positions for worm elements
int theworm_wormpos_x[WORM_LENGTH];
// Last usable index into the arrays
// theworm_wormpos_y and theworm_wormpos_x
int theworm_maxindex;
// An index into the array for the worm’s head position
// 0 <= theworm_headindex <= theworm_maxindex
int theworm_headindex;


// The following functions all depend on the model of the worm

// Initialize the worms
enum ResCodes initializeWorm(int headpos_y, int headpos_x, enum WormHeading dir, enum ColorPairs color, int len_max) {
    // Initialize position of worms head
    // theworm_headpos_y = headpos_y;
    // theworm_headpos_x = headpos_x;

    theworm_maxindex = len_max - 1;
    theworm_headindex = 0;

    for (int i = 0; i < theworm_maxindex; i++) {
        theworm_wormpos_x[i] = UNUSED_POS_ELEM;
        theworm_wormpos_y[i] = UNUSED_POS_ELEM;
    }

    theworm_wormpos_x[theworm_headindex] = headpos_x;
    theworm_wormpos_y[theworm_headindex] = headpos_y;

    // Initialize the heading of the worm
    setWormHeading(dir);

    // Initialze color of the worm
    theworm_wcolor = color;
    return RES_OK;
}

// Show the worms's elements on the display
// Simple version
void showWorm(bool dead) {
    // Due to our encoding we just need to show the head element
    // All other elements are already displayed
    if (dead) 
        placeItem(theworm_wormpos_y[theworm_headindex], theworm_wormpos_x[theworm_headindex], SYMBOL_WORM_INNER_ELEMENT, COLP_DATA);
    else
    	placeItem(theworm_wormpos_y[theworm_headindex], theworm_wormpos_x[theworm_headindex], SYMBOL_WORM_INNER_ELEMENT, COLP_USER_WORM);
}

void cleanWormTail() {
    // Compute tailindex
    int tailindex = (theworm_headindex + 1) % theworm_maxindex;
    // Check the array of worm elements.
    // Is the array element at tailindex already in use?
    // Checking either array theworm_wormpos_y
    // or theworm_wormpos_x is enough.
    if (theworm_wormpos_y[tailindex] != UNUSED_POS_ELEM) {
        // YES: place a SYMBOL_FREE_CELL at the tail's position
        placeItem(theworm_wormpos_y[tailindex], theworm_wormpos_x[tailindex], SYMBOL_FREE_CELL, COLP_FREE_CELL);
    }
}

void moveWorm(enum GameStates *agame_state) {
    // Compute and store new head position according to current heading.
    // theworm_headpos_y += theworm_dy;
    // theworm_headpos_x += theworm_dx;

    int headpos_x = theworm_wormpos_x[theworm_headindex] + theworm_dx;
    int headpos_y = theworm_wormpos_y[theworm_headindex] + theworm_dy;

    attron(COLOR_PAIR(COLP_FREE_CELL));
    mvprintw(getLastRow(), 40, "         ");
    attroff(COLOR_PAIR(COLP_FREE_CELL));
    attron(COLOR_PAIR(COLP_DATA));
    mvprintw(getLastRow(), 40, "%dx%d %d", headpos_y, headpos_x, theworm_headindex);
    attroff(COLOR_PAIR(COLP_DATA));

    // Check if we would leave the display if we move the worm's head according
    // to worm's last direction.
    // We are not allowed to leave the display's window.
    if (headpos_x < 0) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos_x > getLastCol()) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos_y < 0) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos_y > getLastRow()) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else {
        // We will stay within bounds.
        if (isInUseByWorm(headpos_y, headpos_x)) {
            // That's bad: stop game
            *agame_state = WORM_CROSSING;
        }
    }
    // Check the status of *agame_state
    // Go on if nothing bad happened
    if (*agame_state == WORM_GAME_ONGOING) {
        // So all is well: we did not hit anything bad and did not leave the
        // window. --> Update the worm structure.
        // Increment theworm_headindex
        // Go round if end of worm is reached (ring buffer)
        theworm_headindex = (theworm_headindex + 1) % theworm_maxindex;
        // Store new coordinates of head element in worm structure
        theworm_wormpos_x[theworm_headindex] = headpos_x;
        theworm_wormpos_y[theworm_headindex] = headpos_y;
    }
}

// A simple collision detection
bool isInUseByWorm(int new_headpos_y, int new_headpos_x) {
    // bool collision = false;

    for (int i = 0;i <= theworm_maxindex; i++) {
    	// if theworm_wormpos_x[i] == UNUSED_POS_ELEM theworm_wormpos_y[i] == UNUSED_POS_ELEM is also true
        if (theworm_wormpos_x[i] == UNUSED_POS_ELEM) {
        	// break;
            return false;
        }
        
        if (theworm_wormpos_x[i] == new_headpos_x && theworm_wormpos_y[i] == new_headpos_y) {
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
void setWormHeading(enum WormHeading dir) {
    switch (dir) {
        case WORM_UP :// User wants up
            theworm_dx = 0;
            theworm_dy = -1;
            break;
        case WORM_DOWN :// User wants down
            theworm_dx = 0;
            theworm_dy = 1;
            break;
        case WORM_LEFT:// User wants left
            theworm_dx = -1;
            theworm_dy = 0;
            break;
        case WORM_RIGHT:// User wants right
            theworm_dx = 1;
            theworm_dy = 0;
            break;
    }
}
