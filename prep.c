#include "prep.h"

#include <curses.h>
#include <locale.h>

// Initialize application with respect to curses settings
void initializeCursesApplication() {
    setlocale(LC_ALL, "");
    initscr(); // Initialize the curses screen

    // Note:
    // The call to initscr() defines various global variables of the curses
    // framework. stdscr, LINES, COLS, TRUE, FALSE

    noecho(); // Characters typed ar not echoed
    cbreak(); // No buffering of stdin
    nonl();   // Do not translate 'return key' on keyboard to newline character
    keypad(stdscr, TRUE); // Enable the keypad
    curs_set(0);          // Make cursor invisible
    // Begin in non-single-step mode (getch will not block)
    nodelay(stdscr, TRUE); // make getch to be a non-blocking call
}

// Reset display to normale state and terminate curses application
void cleanupCursesApp(void) {
    standend();  // Turn off all attributes
    refresh();   // Write changes to terminal
    curs_set(1); // Set cursor state to normal visibility
    endwin();    // Terminate curses application
}
