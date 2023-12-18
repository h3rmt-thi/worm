#include <curses.h>
#include <stdbool.h>
#include <stdio.h>

#include "board_model.h"
#include "messages.h"
#include "prep.h"
#include "worm.h"
#include "worm_model.h"

// void initializeColors();

// void readUserInput(enum GameStates *agame_state);

// enum ResCodes doLevel();

// Initialize colors of the game
void initializeColors() {
    // Define colors of the game
    start_color();
    init_pair(COLP_FREE_CELL, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLP_DATA, COLOR_RED, COLOR_BLACK);
    init_pair(COLP_BARRIER, COLOR_RED, COLOR_CYAN);
    init_pair(COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
}

void readUserInput(struct Worm *worm, enum GameStates *agame_state) {
    int ch; // For storing the key codes

    if ((ch = getch()) > 0) {
        // Is there some user input?
        // Blocking or non-blocking depends of config of getch
        switch (ch) {
        case 'q': // User wants to end the show
            *agame_state = WORM_GAME_QUIT;
            break;
        case KEY_UP: // User wants up
            setWormHeading(worm, WORM_UP);
            break;
        case KEY_DOWN: // User wants down
            setWormHeading(worm, WORM_DOWN);
            break;
        case KEY_LEFT: // User wants left
            setWormHeading(worm, WORM_LEFT);
            break;
        case KEY_RIGHT: // User wants right
            setWormHeading(worm, WORM_RIGHT);
            break;
        case 's': // User wants single step
            nodelay(stdscr, FALSE);
            break;
        case ' ': // Terminate single step; make getch non-blocking again
            nodelay(stdscr, TRUE);
            break;
        default:;
        }
    }
}

enum ResCodes doLevel() {
    struct Worm userworm;
    enum GameStates game_state = WORM_GAME_ONGOING; // The current game_state

    // There is always an initialized user worm.
    // Initialize the userworm with its size, position, heading.
    const struct Pos start = {getLastRow(), 0};

    enum ResCodes res_code = initializeWorm(&userworm, start, WORM_RIGHT,
                                            COLP_USER_WORM, WORM_LENGTH);
    if (res_code != RES_OK) {
        return res_code;
    }

    showBorderLine();
    showWorm(&userworm, false);

    // Display all what we have set up until now
    refresh();

    // Start the loop for this level
    bool end_level_loop = false; // Flag for controlling the main loop
    while (!end_level_loop) {
        // Process optional user input
        readUserInput(&userworm, &game_state);
        if (game_state == WORM_GAME_QUIT) {
            end_level_loop = true;
            continue;
        }

        // Process userworm
        // Clean the tail of the worm
        cleanWormTail(&userworm);
        // Now move the worm for one step
        moveWorm(&userworm, &game_state);
        // Show the worm at its new position
        showWorm(&userworm, false);
        // END process userworm

        // Bail out of the loop if something bad happened
        if (game_state != WORM_GAME_ONGOING) {
            end_level_loop = true;
            // placeItem(theworm_wormpos_y[theworm_headindex],
            // theworm_wormpos_x[theworm_headindex], SYMBOL_WORM_INNER_ELEMENT,
            // COLP_DATA);
            showWorm(&userworm, true);
            refresh();
            napms(NAP_TIME * 10);
            continue;
        }

        // Sleep a bit before we show the updated window
        napms(NAP_TIME);

        // Display all the updates
        refresh();

        // Start next iteration
    }

    // Preset res_code for rest of the function
    res_code = RES_OK;

    switch (game_state) {
    case WORM_GAME_QUIT:
        // User must have typed 'q' for quit
        showDialog("Sie haben die aktuelle Runde abgebrochen!",
                   "Bitte Taste druecken");
        break;
    case WORM_OUT_OF_BOUNDS:
        showDialog("Sie haben das Spiel verloren,"
                   " weil Sie das Spielfeld verlassen haben",
                   "Bitte Taste druecken");
        break;
    case WORM_CROSSING:
        showDialog("Sie haben das Spiel verloren,"
                   " weil Sie einen Wurm gekreuzt haben",
                   "Bitte Taste druecken");
        break;
    default:
        showDialog("Interner Fehler!", "Bitte Taste druecken");
        // Set error result code. This should never happen.
        res_code = RES_INTERNAL_ERROR;
    }

    // For some reason we left the control loop of the current level.
    // However, in this version we do not yet check for the reason.
    // There is no user feedback at the moment!

    // Normal exit point
    return res_code;
}

int main(void) {
    int res_code; // Result code from functions

    // Here we start
    initializeCursesApplication(); // Init various settings of our application
    initializeColors();            // Init colors used in the game

    // Maximal LINES and COLS are set by curses for the current window size.
    // Note: we do not cope with resizing in this simple examples!

    // Check if the window is large enough to display messages in the message
    // area a has space for at least one line for the worm
    if (LINES < MA_ROWS_RESERVED + MIN_NUMBER_OF_ROWS || COLS < MIN_NUMBER_OF_COLS) {
        // Since we not even have the space for displaying messages
        // we print a conventional error message via printf after
        // the call of cleanupCursesApp()
        cleanupCursesApp();
        printf("Das Fenster ist zu klein: wir brauchen mindestens %dx%d\n",
               MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS);
        res_code = RES_FAILED;
    } else {
        res_code = doLevel();
        cleanupCursesApp();
    }

    return res_code;
}
