
#include "graphics.h"

void drawRect(Screen * s, Point pos, Point size);
void drawLine(Screen * s, Point p1, Point p2);
void drawPath(Screen * s, Point * path, int len);
void clearPath(Screen * s, Point * path, int len);
void clearBuffer(Screen * s);
#ifdef UNICORN
int animation = 0;
#endif
int scrPos(Screen * s, int x, int y) {
    return y * (s->width + 1) + x;
}

void putLine(Screen * s, int x, int y, char c) {
    s->bufferChar[scrPos(s, x, y)] = c;
}

void drawPathChar(Screen * s, int x, int y, char c) {
    if (x + 1 > 0 && x + 1 < s->width - MINION_LIST_WIDTH && y + 1 > 0 && y + 1 < s->height - 1 - FEEDBACK_HEIGHT) {
        s->bufferChar[scrPos(s, x + 1, y + 1)] = c;
    }
}
void drawPathHilight(Screen * s, int x, int y, int c) {
    if (x + 1 > 0 && x + 1 < s->width - MINION_LIST_WIDTH && y + 1 > 0 && y + 1 < s->height - 1 - FEEDBACK_HEIGHT) {
        s->bufferBColour[scrPos(s, x + 1, y + 1)] = c;
    }
}

void drawPathCharColour(Screen * s, int x, int y, char c, int fColour, int bColour) {
    if (x > 0 && x < s->width - MINION_LIST_WIDTH && y > 0 && y < s->height - 1 - FEEDBACK_HEIGHT) {
        s->bufferChar[scrPos(s, x + 1, y + 1)] = c;
        s->bufferFColour[scrPos(s, x + 1, y + 1)] = fColour;
        s->bufferBColour[scrPos(s, x + 1, y + 1)] = bColour;
    }
}

int getColourPair(int fColour, int bColour) {
    return fColour * 10 + bColour;
}

#ifndef _WIN32

void initColourPairs() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            init_pair(getColourPair(i, j), i, j);
        }
    }
}

#endif

void initScreen(Screen * s) {

    s->width = 127;
    s->height = 39;
#ifdef __APPLE__
#ifdef UNICORN
    //system("osascript -e \"tell application \\\'Terminal\\\' to set the font name of window 1 to \\\'Lucida Console Unicorn\\\'\"");
        system("osascript -e \"tell application \\\"Terminal\\\" to set the font name of window 1 to \\\"Lucida Console Unicorn\\\"\"");
#endif
#endif
#ifdef _WIN32

#else
    initscr();
    noecho();
    start_color();
    initColourPairs();
    curs_set(0);
#endif
    s->bufferChar = (char *) malloc((s->width + 1) * (s->height + 1));
    s->bufferFColour = (char *) malloc((s->width + 1) * (s->height + 1));
    s->bufferBColour = (char *) malloc((s->width + 1) * (s->height + 1));

    clearBuffer(s);
}

void drawFrame(Screen * s, int windowMode) {
    if (windowMode == WINDOW_GAME) {
        int minListW = MINION_LIST_WIDTH;
        int displayH = FEEDBACK_HEIGHT;
        drawRect(s, newPoint(0, 0), newPoint(s->width, s->height));
        drawLine(s, newPoint(s->width - minListW, 1), newPoint(s->width - minListW, s->height - 1));
        drawLine(s, newPoint(1, s->height - 1 - displayH), newPoint(s->width - 1 - minListW, s->height - 1 - displayH));
    } else if (windowMode == WINDOW_MENU) {
        int menuW = 30;
        int menuH = 10;
        drawString(s, "   _____                                               ____ ___      .__                                  \n  /  _  \\__  _  __ ____   __________   _____   ____   |    |   \\____ |__| ____  ___________  ____   ______\n /  /_\\  \\ \\/ \\/ // __ \\ /  ___/  _ \\ /     \\_/ __ \\  |    |   /    \\|  |/ ___\\/  _ \\_  __ \\/    \\ /  ___/\n/    |    \\     /\\  ___/ \\___ (  <_> )  Y Y  \\  ___/  |    |  /   |  \\  \\  \\__(  <_> )  | \\/   |  \\\\___ \\ \n\\____|__  /\\/\\_/  \\___  >____  >____/|__|_|  /\\___  > |______/|___|  /__|\\___  >____/|__|  |___|  /____  >\n        \\/            \\/     \\/            \\/     \\/               \\/        \\/                 \\/     \\/ ", (s->width - 107) / 2, 5);
        drawRect(s, newPoint(0, 0), newPoint(s->width, s->height));
        drawRect(s, newPoint(s->width / 2 - menuW / 2, s->height / 2 - menuH / 2), newPoint(menuW, menuH));
    } else if (windowMode == 3) {
        drawRect(s, newPoint(0, 0), newPoint(s->width, s->height));
    }
}

void drawRect(Screen * s, Point pos, Point size) {
    char hline = H_LINE; //205
    char vline = V_LINE; //186
    for (int i = 0; i < size.x; i++) {
        s->bufferChar[scrPos(s, pos.x + i, pos.y)] = hline;
        s->bufferChar[scrPos(s, pos.x + i, pos.y + size.y)] = hline;
    }
    for (int i = 0; i < size.y; i++) {
        s->bufferChar[scrPos(s, pos.x, pos.y + i)] = vline;
        s->bufferChar[scrPos(s, pos.x + size.x, pos.y + i)] = vline;
    }

    s->bufferChar[scrPos(s, pos.x, pos.y)] = TL_CORNER;
    s->bufferChar[scrPos(s, pos.x, pos.y + size.y)] = BL_CORNER;
    s->bufferChar[scrPos(s, pos.x + size.x, pos.y)] = TR_CORNER;
    s->bufferChar[scrPos(s, pos.x + size.x, pos.y + size.y)] = BR_CORNER;
}

void drawLine(Screen * s, Point p1, Point p2) {
    char hline = H_LINE;
    char vline = V_LINE;
    if (p1.x < p2.x) {
        for (int i = p1.x; i <= p2.x; i++) {
            s->bufferChar[scrPos(s, i, p1.y)] = hline;
            //putLine(s, i, p1.y, hline);
        }
    } else if (p1.x > p2.x) {
        for (int i = p2.x; i <= p1.x; i++) {
            s->bufferChar[scrPos(s, i, p1.y)] = hline;
            //putLine(s, i, p1.y, hline);
        }
    }

    if (p1.y < p2.y) {
        for (int i = p1.y; i <= p2.y; i++) {
            s->bufferChar[scrPos(s, p1.x, i)] = vline;
            //putLine(s, p1.x, i, vline);
        }
    } else if (p1.y > p2.y) {
        for (int i = p2.y; i <= p1.y; i++) {
            s->bufferChar[scrPos(s, p1.x, i)] = vline;
            //putLine(s, p1.x, i, vline);
        }
    }
}

void drawMap(Screen * s, Level l) {
    drawPath(s, l.path1, l.path1Length);
    drawPath(s, l.path2, l.path2Length);
    drawPath(s, l.path3, l.path3Length);
    
    clearPath(s, l.path1, l.path1Length);
    clearPath(s, l.path2, l.path2Length);
    clearPath(s, l.path3, l.path3Length);
}

void drawPath(Screen * s, Point * path, int len) {
    int direction;
    /*
      2
      |
    1-+-3
      |
      4
     */
    int hChar = H_LINE; //(char)201;//
    int vChar = V_LINE; //(char)179;//
    for (int i = 1; i < len; ++i) {
        if (i > 0) {
            if (path[i].x == path[i - 1].x) {
                if (path[i].y < path[i - 1].y) {
                    direction = 2;
                }
                if (path[i].y > path[i - 1].y) {
                    direction = 4;
                }
            } else if (path[i].x > path[i - 1].x) {
                direction = 3;
            } else if (path[i].x < path[i - 1].x) {
                direction = 1;
            }
        }
        if (direction == 1) {
            for (int j = 0; j < 5; j++) {
                drawPathChar(s, path[i].x + j, path[i].y - 1, hChar);
                drawPathChar(s, path[i].x + j, path[i].y + 2, hChar);
            }
            for (int j = 0; j <= 2; j++) {
                drawPathChar(s, path[i].x - 1, path[i].y + j, vChar);
            }
            drawPathChar(s, path[i].x - 1, path[i].y - 1, TL_CORNER);
            drawPathChar(s, path[i].x - 1, path[i].y + 2, BL_CORNER);
        } else if (direction == 2) {
            for (int j = 0; j < 4; j++) {
                drawPathChar(s, path[i].x + j, path[i].y - 1, hChar);
            }
            for (int j = 0; j <= 0; j++) {
                drawPathChar(s, path[i].x - 1, path[i].y + j, vChar);
                drawPathChar(s, path[i].x + 4, path[i].y + j, vChar);
            }
            drawPathChar(s, path[i].x + 4, path[i].y - 1, TR_CORNER);
            drawPathChar(s, path[i].x - 1, path[i].y - 1, TL_CORNER);
        } else if (direction == 3) {
            for (int j = -1; j < 4; j++) {
                drawPathChar(s, path[i].x + j, path[i].y - 1, hChar);
                drawPathChar(s, path[i].x + j, path[i].y + 2, hChar);
            }
            for (int j = 0; j <= 2; j++) {
                drawPathChar(s, path[i].x + 4, path[i].y + j, vChar);
            }
            drawPathChar(s, path[i].x + 4, path[i].y - 1, TR_CORNER);
            drawPathChar(s, path[i].x + 4, path[i].y + 2, BR_CORNER);
        } else if (direction == 4) {
            for (int j = -1; j < 4; j++) {
                drawPathChar(s, path[i].x + j, path[i].y + 2, hChar);
            }
            for (int j = 0; j <= 2; j++) {
                drawPathChar(s, path[i].x - 1, path[i].y + j, vChar);
                drawPathChar(s, path[i].x + 4, path[i].y + j, vChar);
            }
            drawPathChar(s, path[i].x + 4, path[i].y + 2, BR_CORNER);
            drawPathChar(s, path[i].x - 1, path[i].y + 2, BL_CORNER);
        }
    }
}

void clearPath(Screen * s, Point * path, int len) {

    int direction = 0;
    int oldDirection = 0;

    for (int i = 0; i < len; i++) {
        if (i > 0) {
            if (path[i].x == path[i - 1].x) {
                if (path[i].y < path[i - 1].y) {
                    direction = 2;
                }
                if (path[i].y > path[i - 1].y) {
                    direction = 4;
                }
            } else if (path[i].x > path[i - 1].x) {
                direction = 3;
            } else if (path[i].x < path[i - 1].x) {
                direction = 1;
            }
        }

        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 2; k++) {
                drawPathChar(s, path[i].x + j, path[i].y + k, ' ');
            }
        }

        /*
          2
          |
        1-+-3
          |
          4
         */

        if (direction == 1 && oldDirection == 2) {
            drawPathChar(s, path[i].x, path[i].y + 2, TR_CORNER);
        } else if (direction == 2 && oldDirection == 3) {
            drawPathChar(s, path[i].x - 1, path[i].y, TL_CORNER);
        } else if (direction == 3 && oldDirection == 4) {
            drawPathChar(s, path[i].x + 3, path[i].y - 1, BL_CORNER);
        } else if (direction == 4 && oldDirection == 1) {
            drawPathChar(s, path[i].x + 4, path[i].y + 1, BR_CORNER);

        } else if (direction == 2 && oldDirection == 1) {
            drawPathChar(s, path[i].x + 4, path[i].y, TR_CORNER);
        } else if (direction == 3 && oldDirection == 2) {
            drawPathChar(s, path[i].x + 3, path[i].y + 2, TL_CORNER);
        } else if (direction == 4 && oldDirection == 3) {
            drawPathChar(s, path[i].x - 1, path[i].y + 1, BL_CORNER);
        } else if (direction == 1 && oldDirection == 4) {
            drawPathChar(s, path[i].x, path[i].y - 1, BR_CORNER);
        }
        oldDirection = direction;

    }
}

void highlightPath(Screen * s, Point * path, int len, int colour) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 2; k++) {
                drawPathHilight(s, path[i].x + j + 0, path[i].y + k + 0, colour);
                //s->bufferBColour[scrPos(s, path[i].x + j + 1, path[i].y + k + 1)] = colour;
            }
        }
    }
}


void highlightMinion(Screen * s, Minion m, int colour) {
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 2; k++) {
            s->bufferBColour[scrPos(s, m.path[(int) m.pos].x + j + 1, m.path[(int) m.pos].y + k + 1)] = colour;
        }
    }

}

void drawString(Screen * s, char * str, int x, int y) {
    int col = 0;
    int row = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != '\n') {
            s->bufferChar[scrPos(s, x + col, y + row)] = str[i];
            col++;
        } else {
            row++;
            col = 0;
        }
    }
}

void drawStringWrap(Screen * s, char * str, int x, int y, int w) {

    int col = 0;
    int row = 0;
    int nextSpace = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\n') {
            row++;
            col = 0;
        } else if (str[i] == ' ') {
            char c = 'g';
            nextSpace = 0;
            while (c != ' ' && c != '\0') {
                nextSpace++;
                c = str[col + nextSpace];
            }
            if (col + nextSpace >= w) {
                row++;
                col = 0;
            } else {
                s->bufferChar[scrPos(s, x + col, y + row)] = str[i];
                col++;
            }
        } else {
            s->bufferChar[scrPos(s, x + col, y + row)] = str[i];
            col++;
        }
    }
}

void drawHilightStr(Screen * s, char * str, int x, int y, int colour) {
    int col = 0;
    int row = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != '\n') {
            s->bufferChar[scrPos(s, x + col, y + row)] = str[i];
            s->bufferBColour[scrPos(s, x + col, y + row)] = colour;
            col++;
        } else {
            row++;
            col = 0;
        }
    }
}

void drawColourStr(Screen * s, char * str, int x, int y, int colour) {
    int col = 0;
    int row = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != '\n') {
            s->bufferChar[scrPos(s, x + col, y + row)] = str[i];
            s->bufferFColour[scrPos(s, x + col, y + row)] = colour;
            col++;
        } else {
            row++;
            col = 0;
        }
    }
}

void drawHilight(Screen * s, int length, int x, int y, int colour) {
    for (int i = 0; i < length; i++) {
        s->bufferBColour[scrPos(s, x + i, y)] = colour;
    }
}

void drawMinion(Screen * s, Minion m) {
#ifdef UNICORN
    if(animation){
        drawColourStr(s, "@   \n    ", m.path[(int) m.pos].x + 1, m.path[(int) m.pos].y + 1, m.fColour[0]);
        animation = 0;
    }else{
        drawColourStr(s, "#   \n    ", m.path[(int) m.pos].x + 1, m.path[(int) m.pos].y + 1, m.fColour[0]);
        animation = 1;
    }
#else
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            drawPathCharColour(s, m.path[(int) m.pos].x + i, m.path[(int) m.pos].y + j, m.image[j * 4 + i], m.fColour[j * 4 + i], m.bColour[j * 4 + i]);
            //s->bufferFColour[scrPos(s, m.path[(int)m.pos].x + i, m.path[(int)m.pos].y + j)] = m.fColour[j * 4 + i];
            //s->bufferBColour[scrPos(s, m.path[(int)m.pos].x + i, m.path[(int)m.pos].y + j)] = m.bColour[j * 4 + i];
        }
    }
#endif
}

void drawTower(Screen * s, Tower t) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            drawPathCharColour(s, t.pos.x + i, t.pos.y + j, t.image[j * 4 + i], t.fColour[j * 4 + i], t.bColour[j * 4 + i]);
            //s->bufferFColour[scrPos(s, t.pos.x + i, t.pos.y + j)] = t.fColour[j * 4 + i];
            //s->bufferBColour[scrPos(s, t.pos.x + i, t.pos.y + j)] = t.bColour[j * 4 + i];
        }
    }
}

void updateGraphics(Screen * s) {
    for (int y = 0; y <= s->height; y++) {
        for (int x = 0; x <= s->width; x++) {
#ifdef _WIN32
            printf("%c", s->bufferChar[scrPos(s, x, y)]);
#else
            attron(COLOR_PAIR(getColourPair(s->bufferFColour[scrPos(s, x, y)], s->bufferBColour[scrPos(s, x, y)])));
            mvaddch(y, x, s->bufferChar[scrPos(s, x, y)]);
            attroff(COLOR_PAIR(getColourPair(s->bufferFColour[scrPos(s, x, y)], s->bufferBColour[scrPos(s, x, y)])));
#endif
        }
#ifdef _WIN32
        printf("\n");
#endif
    }

    clearBuffer(s);

}

void clearBuffer(Screen * s) {
    for (int y = 0; y <= s->height; y++) {
        for (int x = 0; x <= s->width; x++) {
            s->bufferChar[scrPos(s, x, y)] = ' ';
#ifndef _WIN32
            s->bufferFColour[scrPos(s, x, y)] = COLOR_BLACK;
            s->bufferBColour[scrPos(s, x, y)] = COLOR_WHITE;
#endif
        }
    }
}

void destroyScreen(Screen * s) {

#ifdef _WIN32

#else
    curs_set(1  );
    endwin();
    clear();
#endif

#ifdef __APPLE__
#ifdef UNICORN
    //system("osascript -e \"tell application \\\'Terminal\\\' to set the font name of window 1 to \\\'Menlo Regular\\\'\"");
        system("osascript -e \"tell application \\\"Terminal\\\" to set the font name of window 1 to \\\"Menlo Regular\\\"\"");
#endif
#endif
    free(s->bufferChar);
    free(s->bufferFColour);
    free(s->bufferBColour);
    //free(s);
}
