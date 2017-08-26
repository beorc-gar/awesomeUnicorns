#include "awesomeUnicorns.h"

int main(void) {
    int gameFinished;
    int levelNum;
    int numMinions;
    int i;
    int selection;
    int menu;
    Level* levels;
    Minion* minions;

    /*Screen displayed during program execution*/
    Screen GameScreen;


    initScreen(&GameScreen);

	curs_set(0);
    gameFinished = 0;
    levelNum = getLevel();
    selection = 0;
    menu = 1;

    levels = loadLevels();  /*Load and parse level files in here*/

    while (!gameFinished && levelNum != -1) {

        /*Enter into main menu*/
        while (levelNum == getLevel() && menu) {
            levelNum = menuMode(levels, &GameScreen);
        } 
        if (levelNum == -1) {
            break;
        }
        menu = 0;
        minions = malloc(sizeof(Minion)*MAXMINIONS);
        numMinions = 0;

        /*Game as started, let them pick minions*/
        menu = selectionMode(levels[levelNum], &minions, &numMinions, &GameScreen);
        
        /*User selected to go back to menu*/
        if (menu) {
            for (i = 0; i < numMinions; i++) {
                destroyMinion(&minions[i]);
            }
            free(minions);
            levelNum = getLevel();
            updateGraphics(&GameScreen);
            continue;
        }
        
        /*User did not select a large enough number of minions*/
        if (numMinions < levels[levelNum].minionsToWin) {
            selection = notEnough(&GameScreen);
            for (i = 0; i < numMinions; i++) {
                destroyMinion(&minions[i]);
            }
            free(minions);
            if (selection == 0)
                continue;
            else if (selection == 1) {
                levelNum = getLevel();
                menu = 1;
                continue;
            } else {
                break;
            }
        }
#ifndef NOGAME
        /*Minion selection is finished, start simulation*/
        gameFinished = simulation(levels[levelNum], minions, numMinions, &GameScreen);
        levelNum += gameFinished;
#endif
        /*User beat the last level*/
        if (levelNum == NUMBER_OF_LEVELS) {
            int height, width, startx, starty;
            char input;
            input = ' ';
            getmaxyx(stdscr, height, width);
            starty = (LINES - height) /2;
            startx = (COLS - width)/2;
            WINDOW *win = newwin(height, width, starty, startx);
            gameWinScreen(win);
            while (input != '\n') {
                input = wgetch(win);
            }
            delwin(win);
            
            levelNum = getLevel();
            menu = 1;
            gameFinished = 0;
            for (i = 0; i < numMinions; i++) {
                destroyMinion(&minions[i]);
            }
            free(minions);
            continue;
        }

        for (i = 0; i < numMinions; i++) {
            destroyMinion(&minions[i]);
        }
        free(minions);

        if (!gameFinished) {
            gameFinished = gameOverL(&GameScreen);
        } else {
            gameFinished = gameOverW(&GameScreen);
            if (levelNum+1 >= getLevel())
                unlockLevel(levelNum+1);
        }

        if (gameFinished == 0) {
            continue;
        } else if (gameFinished == 1) {
                levelNum = getLevel();
                menu = 1;
                gameFinished = 0;
                continue;
        } else {
            break;
        }
    }

    free(levels);
    destroyScreen(&GameScreen);

    return 0;
}
