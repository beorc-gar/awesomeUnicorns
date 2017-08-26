#ifndef GRAPHICS_BIBBY_ADAM
#define GRAPHICS_BIBBY_ADAM

#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "structs.h"
#include  <string.h>


#define MINION_LIST_WIDTH 20
#define FEEDBACK_HEIGHT 5

#ifdef _WIN32

#else
#include <ncurses.h>
#endif

#define WINDOW_GAME 0
#define WINDOW_MENU 1


#ifdef _WIN32

#define H_LINE (char) 196
#define V_LINE (char) 179
#define TR_CORNER (char) 191
#define TL_CORNER (char) 218
#define BR_CORNER (char) 217
#define BL_CORNER (char) 192

#else
#define H_LINE '-'
#define V_LINE '|'

#ifdef UNICORN
#define TR_CORNER '$'
#define TL_CORNER '%'
#define BR_CORNER '^'
#define BL_CORNER '&'
#else
#define TR_CORNER '+'
#define TL_CORNER '+'
#define BR_CORNER '+'
#define BL_CORNER '+'
#endif
#include <ncurses.h>

#endif


typedef struct scr {
    char * bufferChar;
    char * bufferFColour;
    char * bufferBColour;
    int windowMode;
    int width;
    int height;
} Screen;


/*
initiolizes the three buffers in the screen and initiolizes ncurses

The first command to run on each screen
*/
void initScreen(Screen * s);

/*
Draws the border around the screen

windowMode = WINDOW_GAME or WINDOW_MENU
*/
void drawFrame(Screen * s, int windowMode);

/*
draws the paths on the screen
*/
void drawMap(Screen * s, Level l);

/*
draws a string at postion x, y
*/
void drawString(Screen * s, char * str, int x, int y);

/*
draws a string at postion x, y and it will wrap if it is longer than w charaters
*/
void drawStringWrap(Screen * s, char * str, int x, int y, int w);

/*
draws a string at postion x, y with a with the bacground colour 'colour'

Colours are defined in ncurses
*/
void drawHilightStr(Screen * s, char * str, int x, int y, int colour);

/*
changes the bacground colour from x,y to x+length,y to 'colour'

Colours are defined in ncurses
*/
void drawHilight(Screen * s, int length, int x, int y, int colour);

void highlightPath(Screen * s, Point * path, int len, int colour);

void highlightMinion(Screen * s, Minion m, int colour);

/*
draws the minion m
*/
void drawMinion(Screen * s, Minion m);

/*
draws the tower t
*/
void drawTower(Screen * s, Tower t);

/*
pushed the buffers to the screen then clears them

run when you want to upadate the image on the screen
*/
void updateGraphics(Screen * s);

/*
frees the memory related to the screen and turns off ncurses

run at the end of the program
*/
void destroyScreen(Screen * s);

#endif
