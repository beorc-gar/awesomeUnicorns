#include "structs.h"
#include "graphics.h"
#include "point.h"
#include "tower.h"
#include "minion.h"
#include "fileParsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#ifndef AWESOMEUNICORNS_H_BUCKLEY_DAVID
#define AWESOMEUNICORNS_H_BUCKLEY_DAVID

/* Puts game into the main menu mode.
 * Params:     A level struct that will be filled with information from the level file
            Screen to display
 * Returns: index of selected level struct */
extern int menuMode(Level* level, Screen* screen);

/* Puts game into the minion selection mode
 * Params:     A level struct to display
             An array of minions to fill
             Screen
             A pointer to an integer that will be the number of minions
 * Returns: 0 - Play simulation
            1 - Return to menu -  */
extern int selectionMode(Level level, Minion** minions, int* minionNum, Screen* screen);

/* Puts game into the simulation mode
 * Params:     A level struct to display
             An array of minions
             An integer representing the number of minions
             Screen to display
 * Returns: Wheather the level was finished or not
             0 - Game was not completed
             1 - Game was completed*/
extern int simulation(Level level, Minion* minions, int minionNum, Screen* screen);

/* Displays a gameover screen to the user
 * Params:     Screen to display
 * Returns: Wheather the user wants to retry to exit
             0 - Retry
             1 - Exit*/
extern int gameOverL(Screen* screen);
extern int gameOverW(Screen* screen);
extern int notEnough(Screen* screen);

extern void gameWinScreen(WINDOW* win);

extern void destroyLevel(Level* level);
#endif
