#define _BSD_SOURCE
#include "awesomeUnicorns.h"
#define GAMETICK 500000
#define STAGGERED

void printHelp(WINDOW *win);

int menuMode(Level* level, Screen* screen) {
    int index;
    int pos;
    int levels;
    int userInput;
    int i;
    char bLevel[8];
    int input;

    index = 0;
    pos = 0;
    levels = 1;

    drawFrame(screen, WINDOW_MENU);
    drawHilightStr(screen, "Start", 61, 16, yellow);
    drawHilightStr(screen, "Select Level", 58, 18, white);
    drawHilightStr(screen, "Exit", 61, 20, white);
	drawHilightStr(screen, "Instructions", 58, 22, white);
	drawString(screen, "Press w or s (or arrow keys) to navigate the menu", 48, 25);
    drawString(screen, "Press Spacebar to select menu option", 48, 26);
    updateGraphics(screen);
	keypad(stdscr, TRUE);
	curs_set(0);
	
    do
    {
        userInput = getch();
        drawFrame(screen, WINDOW_MENU);

        switch (userInput) {
            case KEY_UP:
            case 'w':
                if (pos != 0)
                    pos -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if (pos != 3)
                    pos += 1;
                break;
        }

        if (pos == 0) {
            drawHilightStr(screen, "Start", 61, 16, yellow);
            drawHilightStr(screen, "Select Level", 58, 18, white);
            drawHilightStr(screen, "Exit", 61, 20, white);
			drawHilightStr(screen, "Instructions", 58, 22, white);
        } else if (pos == 1) {
            drawHilightStr(screen, "Start", 61, 16, white);
            drawHilightStr(screen, "Select Level", 58, 18, yellow);
            drawHilightStr(screen, "Exit", 61, 20, white);
			drawHilightStr(screen, "Instructions", 58, 22, white);
        } else if (pos == 2) {
            drawHilightStr(screen, "Start", 61, 16, white);
            drawHilightStr(screen, "Select Level", 58, 18, white);
            drawHilightStr(screen, "Exit", 61, 20, yellow);
			drawHilightStr(screen, "Instructions", 58, 22, white);
        } else{
            drawHilightStr(screen, "Start", 61, 16, white);
            drawHilightStr(screen, "Select Level", 58, 18, white);
            drawHilightStr(screen, "Exit", 61, 20, white);
			drawHilightStr(screen, "Instructions", 58, 22, yellow);
        }
		drawString(screen, "Press w or s (or arrow keys) to navigate the menu", 48, 25);
		drawString(screen, "Press Spacebar to select menu option", 48, 26);
		
        updateGraphics(screen);
    }while (userInput != ' ');
    
    if (pos == 0) {
        return index;
    } else if (pos == 1) {
        levels = getLevel();
        pos = 0;
        drawFrame(screen, WINDOW_MENU);
        for (i = 0; i < levels; i++) {
            memset(bLevel, 0, sizeof(bLevel));
            sprintf(bLevel, "Level %d", i+1);
            if (pos == i) {
                drawHilightStr(screen, bLevel, 61, 15+(2*i), yellow);
            } else {
                drawHilightStr(screen, bLevel, 61, 15+(2*i), white);
            }
        }
        drawHilightStr(screen, "Back", 61, 15+(2*i), white);
        updateGraphics(screen);

        do {
            userInput = getch();
            drawFrame(screen, WINDOW_MENU);
            switch (userInput) {
                case KEY_UP:
                case 'w':
                    if (pos > 0)
                        pos -= 1;
                    break;
                case KEY_DOWN:
                case 's':
                    if (pos < levels)
                        pos += 1;
                    break;
            }
            for (i = 0; i < levels; i++) {
                memset(bLevel, 0, sizeof(bLevel));
                sprintf(bLevel, "Level %d", i+1);
                if (pos == i) {
                    drawHilightStr(screen, bLevel, 61, 15+(2*i), yellow);
                 } else {
                    drawHilightStr(screen, bLevel, 61, 15+(2*i), white);
                }
            }
            if (pos == levels) {
                drawHilightStr(screen, "Back", 61, 15+(2*i),yellow);
            } else {
                drawHilightStr(screen, "Back", 61, 15+(2*i), white);
            }
            updateGraphics(screen);
        } while (userInput != ' ');

        return pos;
    } else if (pos == 2) {
        return -1;
    } else {
        int height=40, width=128;
        WINDOW *win = newwin(height, width, 0, 0);
        input = 0;
        while (input != '\n') {
            printHelp(win);
            input = wgetch(win);
        }
        
        delwin(win);
		
		return getLevel();
	}
}

void drawSelectionMode(Level level, Screen* screen)
{
    drawFrame(screen, WINDOW_GAME);
    drawMap(screen, level);
    int i, x, y;
    for (i = 0; i < level.towersLength; i++) {
        drawTower(screen, level.towers[i]);
    }
    
    x = 108;
    y = 10;
    char *toWin = malloc(sizeof(char)*20);
    sprintf(toWin, "Unicorns to win: %d", level.minionsToWin);
    drawString(screen,"Normal Unicorn $10", x, y - 2);
    drawString(screen,"Speed Unicorn $15", x, y);
    drawString(screen, "Tank Unicorn $20", x, y + 2);
    drawString(screen, toWin, x, y + 4);
	drawString(screen, "Press h", x, y+12);
	drawString(screen, "for instructions", x, y+14);
    drawString(screen, "Press q", x, y+18);
    drawString(screen, "to return to menu", x, y+20);
    free(toWin);
}

void printHelp(WINDOW *win)
{
    int x, y;
    getmaxyx(win, y, x);
    x = x/2 - 22;
    y = y/2 - 4;
    
	mvwprintw(win, y, x-25, "%s", "The goal of the game is to get a certain number of unicorns per level to the exit of the level.");
    y += 2;
    mvwprintw(win, y, x, "%s", "Use w or s to switch between different unicorns");
    y += 2;
    mvwprintw(win, y, x, "%s", "Use a or d to switch between different paths");
    y += 2;
	mvwprintw(win, y, x, "%s", "You can substitute wasd with the arrow keys");
    y += 2;
    mvwprintw(win, y, x, "%s", "Use the Spacebar to buy a unicorn going down selected path");
    y += 2;
    mvwprintw(win, y, x, "%s", "Press Enter to start the simulation");
    y += 2;
    mvwprintw(win, y, x, "%s", "Press Enter to return back to the game");
}

void updateMoneyMinions(Level level, Screen* screen, int numMinions)
{
	char *money = malloc(sizeof(char)*15);
	char *minions = malloc(sizeof(char)*15);
	int x = 108, y = 16;
	sprintf(money, "Money: %d ", level.money);
	sprintf(minions, "Unicorns: %d", numMinions);
	drawString(screen, money, x, y);
	drawString(screen, minions, x, y + 2);
	
	free(money);
	free(minions); 
}

int selectionMode(Level level, Minion** minions, int* numMinions, Screen* screen) {
#ifndef NOSELECT  
    int minionSelection = 0;
    int pathSelection = 0;
    int cost = 0;
    int userInput;
    int x = 108, y = 10;
    int input;
  
	drawSelectionMode(level, screen);
	updateMoneyMinions(level, screen, *numMinions);
	highlightPath(screen, level.path1, level.path1Length, yellow);
    drawHilight(screen, 20, x - 1, y - 2, yellow);
    updateGraphics(screen);
    keypad(stdscr, TRUE);
	curs_set(0);
    
    /*Do while until user presses space (then the game starts)*/
    do
    {
        userInput = getch();
        drawSelectionMode(level, screen);
        updateMoneyMinions(level, screen, *numMinions);
        switch(userInput)
        {
            case 'w':
            case KEY_UP:
                if(minionSelection != 0)
                {
                    minionSelection--;
                }
				
				else
				{
					minionSelection = 2;
				}
                break;
                
            case 's':
            case KEY_DOWN:
                if(minionSelection != 2)
                {
                    minionSelection++;
                }
				
				else
				{
					minionSelection = 0;
				}
                break;
                
            case 'a':
            case KEY_LEFT:
                if(level.path2Length == 1)
				{
					pathSelection = 0;
				}
				else if(level.path3Length == 1)
                {
                    if(pathSelection == 0)
						pathSelection = 1;
					else
						pathSelection = 0;
                }
				else
				{
					if(pathSelection == 0)
					{
						pathSelection = 2;
					}
					
					else
					{
						pathSelection--;
					}
				}
                break;
                
            case 'd':
            case KEY_RIGHT:
               if(level.path2Length == 1)
				{
					pathSelection = 0;
				}
				else if(level.path3Length == 1)
                {
                    if(pathSelection == 0)
						pathSelection = 1;
					else
						pathSelection = 0;
                }
				else
				{
					if(pathSelection == 2)
					{
						pathSelection = 0;
					}
					
					else
					{
						pathSelection++;
					}
				}
                break;
                
            case 'h': ;
                int height=40, width=128;
                WINDOW *win = newwin(height, width, 0, 0);
                input = 0;
                while (input != '\n') {
                    printHelp(win);
                    input = wgetch(win);
                }
                delwin(win);
                drawSelectionMode(level, screen);
                break;

            case 'q':
                return 1;
                
            case ' ':
                //If level money is greater than cost of minion
                if(minionSelection == 0)
                    cost = NORMAL_COST;
                
                else if (minionSelection == 1)
                    cost = SPEED_COST;
                
                else
                    cost = TANK_COST;
                
                if(level.money >= cost && *numMinions < 100)
                {
                    // Subtract money and increase number of minions
                    level.money -= cost;
                    
                    //Create minion
                    if(pathSelection == 0)
                        (*minions)[*numMinions] = createMinion(minionSelection, level.path1, level.path1Length);
                    
                    else if (pathSelection == 1)
                        (*minions)[*numMinions] = createMinion(minionSelection, level.path2, level.path2Length);
                    
                    else
                        (*minions)[*numMinions] = createMinion(minionSelection, level.path3, level.path3Length);
                    *numMinions = *numMinions + 1;
                    
                    updateMoneyMinions(level, screen, *numMinions);
                }
                break;
				
            default:
                break;
        }
        
        if(minionSelection == 0)
        {
            drawHilight(screen, 18, x, y - 2, yellow);
        }
        
        else if(minionSelection == 1)
        {
            drawHilight(screen, 17, x, y, yellow);
        }
        
        else
        {
            drawHilight(screen, 16, x, y+2, yellow);
        }
        
        if(pathSelection == 0)
		{
			highlightPath(screen, level.path1, level.path1Length, yellow);
		}
            
                    
        else if (pathSelection == 1)
		{
			highlightPath(screen, level.path2, level.path2Length, yellow);
				
		}
            
                    
        else
		{
			highlightPath(screen, level.path3, level.path3Length, yellow);
		}
		
        updateGraphics(screen);
        
    }while(userInput != '\n');

    return 0;
	
#endif

#ifdef NOSELECT
    Minion tmp;
    *minions = realloc(*minions, sizeof(Minion)*5);
    *numMinions = 5;

    tmp = createMinion(speed, level.path1, level.path1Length);
    (*minions)[0] = tmp;

    tmp = createMinion(normal, level.path1, level.path1Length);
    (*minions)[1] = tmp;

    tmp = createMinion(tank, level.path1, level.path1Length);
    (*minions)[2] = tmp;

    tmp = createMinion(speed, level.path1, level.path1Length);
    (*minions)[3] = tmp;

    tmp = createMinion(normal, level.path1, level.path1Length);
    (*minions)[4] = tmp;
#endif

}

int simulation(Level level, Minion* minions, int numMinions, Screen* screen) {
    int minionsOut;
    int i, j;
    int cycles;
    int count;
    int sCount, nCount, tCount;
    int currentNum;
    int minionsInRange;
    int alive;
    Minion** withinRange;
    char minionStr[20];
	int speedUp = 1;
	int input;
	
	halfdelay(1);
	keypad(stdscr, TRUE);
	
    Minion* currentMinions;
    if (numMinions < 1) {
        return 0;
    }
#ifdef STAGGERED
    currentMinions = malloc(sizeof(Minion)*numMinions);
    currentMinions[0] = minions[0];
    currentNum = 1;
#endif

#ifndef STAGGERED
    currentMinions = minions;
    currentNum = numMinions;
#endif
    minionsOut = 0;
    cycles = 0;
    sCount = 0;
    nCount = 0;
    tCount = 0;
    count = 0;
    alive = numMinions;

    drawFrame(screen, WINDOW_GAME);
    drawMap(screen, level);
    for (i = 0; i < currentNum; i++) {
        drawMinion(screen, currentMinions[i]);
    }

    for (i = 0; i < level.towersLength; i++) {
        drawTower(screen, level.towers[i]);
    }
    

    updateGraphics(screen);
#ifndef NOPLAY
    while (alive >= level.minionsToWin-minionsOut) {
        if (minionsOut >= level.minionsToWin) {
            /*Enough minions reached the exit, user wins*/
            free(currentMinions);
            return 1;
        }

        moveMinions(currentMinions, currentNum);


        /*
        Deal with poison tower debuff
        */
        
        updateDebuff(level, currentMinions, currentNum);
        for (i = 0; i < currentNum; i++) {
            if (minionReachExit(currentMinions[i])) {
                minionsOut++;
                for (j = i+1; j < currentNum; j++) {
                    currentMinions[j-1] = currentMinions[j];
                }
                currentNum--;
                alive--;
                /*Remove minion from list*/
            }
        }

        /*for all towers*/
	    resetColour(currentMinions, currentNum);
        for (i = 0; i < level.towersLength; i++) {
            /*Update tower cooldown*/
            decrementCooldown(&level.towers[i]);

            /*Shoot at minions*/
            minionsInRange = 0;
            withinRange = found(&level.towers[i], currentMinions, currentNum, &minionsInRange);
            damage(&level.towers[i], withinRange, minionsInRange);
            free(withinRange);
        }
        
        for (i = 0; i < currentNum; i++) {
            if(currentMinions[i].health <= 0) {
                /*Remove minion from list*/
                /*destroyMinion(&currentMinions[i]);*/
                for (j = i+1; j < currentNum; j++) {
                    currentMinions[j-1] = currentMinions[j];
                }
                currentNum--;
                alive--;
            }
        }

        /*
        Display Level
        */
        sCount = 0;
        nCount = 0;
        tCount = 0;
        drawFrame(screen, WINDOW_GAME);
        drawMap(screen, level);
        for (i = 0; i < level.towersLength; i++) {
            drawTower(screen, level.towers[i]);
        }

        for (i = 0; i < currentNum; i++) {
            drawMinion(screen, currentMinions[i]);
        }

        /*
        Write minion hp info
        */
        for (i = 0; i < currentNum; i++) {
            if (i > 18) {
                break;
            }
            memset(minionStr, 0, sizeof(minionStr));
            switch (currentMinions[i].type) {
                case 1:
                    sCount++;
                    sprintf(minionStr, "Speed %d: %d hp", sCount, currentMinions[i].health);
                    break;
                case 0:
                    nCount++;
                    sprintf(minionStr, "Normal %d: %d hp", nCount, currentMinions[i].health);
                    break;
                case 2:
                    tCount++;
                    sprintf(minionStr, "Tank %d: %d hp", tCount, currentMinions[i].health);
                    break;
            }
        
            drawString(screen, minionStr, 110, 2+(2*i));
        }

        updateGraphics(screen);
#ifdef STAGGERED
        if (cycles != numMinions-1) {
            cycles+=1;
            currentMinions[currentNum] = minions[cycles];
            currentNum += 1;
        }
#endif
        input = getch();
		if(input != ERR)
		{
			if(input == KEY_RIGHT)
			{
                if (speedUp < 32)
				    speedUp *= 2;
			}
			
			if(input == KEY_LEFT)
			{
				if(speedUp > 1)
				{
					speedUp /= 2;
				}
			}
		}
		/*game tick*/
        count++;
		refresh();
        usleep(GAMETICK/speedUp);
    }
#endif
    /*All minions died, user failed*/
    return 0;
}

int gameOverL(Screen* screen) {
    int pos;
    int userInput;
    pos = 0;

    drawFrame(screen, WINDOW_MENU);
    drawHilightStr(screen, "You no longer have enough unicorns to win", 44, 12, white);
    drawHilightStr(screen, "Play Again", 59, 17, yellow);
    drawHilightStr(screen, "Main Menu", 59, 19, white);
    drawHilightStr(screen, "Exit", 61, 21, white);
    updateGraphics(screen);
    do
    {
        userInput = getch();
        drawFrame(screen, WINDOW_MENU);
        drawHilightStr(screen, "You no longer have enough unicorns to win", 44, 12, white);
        switch (userInput) {
            case KEY_UP:
            case 'w':
                if (pos != 0)
                    pos -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if (pos != 2)
                    pos += 1;
                break;
        }

        if (pos == 0) {
            drawHilightStr(screen, "Play Again", 59, 17, yellow);
            drawHilightStr(screen, "Main Menu", 59, 19, white);
            drawHilightStr(screen, "Exit", 61, 21, white);
        } else if (pos == 1) {
            drawHilightStr(screen, "Play Again", 59, 17, white);
            drawHilightStr(screen, "Main Menu", 59, 19, yellow);
            drawHilightStr(screen, "Exit", 61, 21, white);
        } else {
            drawHilightStr(screen, "Play Again", 59, 17, white);
            drawHilightStr(screen, "Main Menu", 59, 19, white);
            drawHilightStr(screen, "Exit", 61, 21, yellow);
        }
        updateGraphics(screen);
    } while (userInput != ' ');

    return pos;
}

int gameOverW(Screen* screen) {
    int pos;
    int userInput;

    pos = 0;
    drawFrame(screen, WINDOW_MENU);
    drawHilightStr(screen, "You Win!", 60, 12, white);
    drawHilightStr(screen, "Play Next Level", 56, 17, yellow);
    drawHilightStr(screen, "Main Menu", 59, 19, white);
    drawHilightStr(screen, "Exit", 61, 21, white);
    updateGraphics(screen);
    do
    {
        userInput = getch();
        drawFrame(screen, WINDOW_MENU);
        drawHilightStr(screen, "You Win!", 60, 12, white);
        switch (userInput) {
            case KEY_UP:
            case 'w':
                if (pos != 0)
                    pos -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if (pos != 2)
                    pos += 1;
                break;
        }

        if (pos == 0) {
            drawHilightStr(screen, "Play Next Level", 56, 17, yellow);
            drawHilightStr(screen, "Main Menu", 59, 19, white);
            drawHilightStr(screen, "Exit", 61, 21, white);
        } else if (pos == 1) {
            drawHilightStr(screen, "Play Next Level", 56, 17, white);
            drawHilightStr(screen, "Main Menu", 59, 19, yellow);
            drawHilightStr(screen, "Exit", 61, 21, white);
        } else {
            drawHilightStr(screen, "Play Next Level", 56, 17, white);
            drawHilightStr(screen, "Main Menu", 59, 19, white);
            drawHilightStr(screen, "Exit", 61, 21, yellow);
        }
        updateGraphics(screen);
    } while (userInput != ' ');
    return pos;
}

int notEnough(Screen* screen) {
    int pos;
    int userInput;

    pos = 0;
    drawFrame(screen, WINDOW_MENU);
    drawHilightStr(screen, "Insufficient Number of Unicorns to finish!", 44, 12, white);
    drawHilightStr(screen, "Retry Selection", 56, 17, yellow);
    drawHilightStr(screen, "Main Menu", 61, 19, white);
    drawHilightStr(screen, "Exit Game", 61, 21, white);
    updateGraphics(screen);
    do
    {
        userInput = getch();
        drawFrame(screen, WINDOW_MENU);
        drawHilightStr(screen, "Insufficient Number of Unicorns to finish!", 44, 12, white);
        switch (userInput) {
            case KEY_UP:
            case 'w':
                if (pos != 0)
                    pos -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if (pos != 2)
                    pos += 1;
                break;
        }

        if (pos == 0) {
            drawHilightStr(screen, "Retry Selection", 56, 17, yellow);
            drawHilightStr(screen, "Main Menu", 61, 19, white);
            drawHilightStr(screen, "Exit Game", 61, 21, white);
        } else if (pos == 1) {
            drawHilightStr(screen, "Retry Selection", 56, 17, white);
            drawHilightStr(screen, "Main Menu", 61, 19, yellow);
            drawHilightStr(screen, "Exit Game", 61, 21, white);
        } else {
            drawHilightStr(screen, "Retry Selection", 56, 17, white);
            drawHilightStr(screen, "Main Menu", 61, 19, white);
            drawHilightStr(screen, "Exit Game", 61, 21, yellow);
        }
        updateGraphics(screen);
    } while (userInput != ' ');
    return pos;
}


void gameWinScreen(WINDOW* win)
{
    int x, y;
    getmaxyx(win, y, x);
    x = x/2 - 22;
    y = y/2 - 4;
    
    mvwprintw(win, y, x, "%s", "You beat the game!");
    y += 2;
    mvwprintw(win, y, x, "%s", "Press Enter to return to the main menu");
}

void destroyLevel(Level* level) {
    int i;
    for (i = 0; i < level->towersLength; i++) {
        destroyTower(&level->towers[i]);
    }
    free(level->towers);
    free(level->path1);
    free(level->path2);
    free(level->path3);
}
