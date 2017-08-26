#include "fileParsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int checkAdjacentOrder(Point p1, Point p2);
static void orderPath(Point * path, int pathLength);
static void parseMap(char * line, int lineNumber, Level * level);
static Tower parseTower(char * line, Level * level);
static void shiftPoints(Point * path, int pathLength);

/*
call to update the current highest level unlocked
when a user unlocks a certain level in order to
update the save file
pass it the new current highest level unlocked as an int
*/
extern void unlockLevel(int level) {
    FILE * saveFile;
    saveFile = fopen(SAVE_FILE,"r+");
    char * levelString = malloc(sizeof(char)*3);
    MALLOC_NULL_POINTER_ERROR(levelString);

    snprintf(levelString, 20, "%d", level);
    fputs(levelString, saveFile); //put the string in the file

    free(levelString); //free the levelString
    fclose(saveFile); //close the file
}

/*
call to return the highest currently unlocked level
returns the highest currently unlocked level as an int
*/
extern int getLevel(void) {
    FILE * saveFile;
    saveFile = fopen(SAVE_FILE,"r");
    int level = 0;
    char * buffer = malloc(sizeof(char) * 31);
    MALLOC_NULL_POINTER_ERROR(buffer);

    fscanf(saveFile, "%d", &level);

    free(buffer); //free the buffer
    fclose(saveFile); //close the file
    return level;
}

/*
call to reset the highest currently unlocked level to 1
*/
extern void resetLevel(void) {
    FILE * saveFile;
    saveFile = fopen(SAVE_FILE,"r+");
    int lowestLevel = 1;
    char * levelString = malloc(sizeof(char)*3);
    MALLOC_NULL_POINTER_ERROR(levelString);

    snprintf(levelString, 20, "%d", lowestLevel);
    fputs(levelString, saveFile); //put the string in the file

    fclose(saveFile); //close the file
}

/*
This is the only function that needs to be called in the main to create levels
It will parse the file and use that data to create the level
structs, organize them in an array, and return it
*/
extern Level * loadLevels(void) {
    FILE * levelFile;
    int lineNumber;
    int mapOver; //just a boolean for determining whether the map has been parsed or not
    int towersOver; //just a boolean for determining whether the towers have been parsed or not
    Level * levels = malloc(sizeof(Level) * NUMBER_OF_LEVELS);
    MALLOC_NULL_POINTER_ERROR(levels);

    for (int a = 1; a < NUMBER_OF_LEVELS+1; a++) { //iterate through each level
        /*getting the correct file name and opening the file*/
        char fileName[100] = "";
        sprintf(fileName, "%s%d.txt", LEVEL_FILE_PREFIX, a);
        levelFile = fopen(fileName,"r");

        /*INITIALIZE VALUES*/
        Level * tempLevel = malloc(sizeof(Level));
        MALLOC_NULL_POINTER_ERROR(tempLevel);
        tempLevel->path1Length = 0;
        tempLevel->path2Length = 0;
        tempLevel->path3Length = 0;
        tempLevel->towersLength = 0;
        tempLevel->path1 = malloc(sizeof(Point));
        MALLOC_NULL_POINTER_ERROR(tempLevel->path1);
        tempLevel->path2 = malloc(sizeof(Point));
        MALLOC_NULL_POINTER_ERROR(tempLevel->path2);
        tempLevel->path3 = malloc(sizeof(Point));
        MALLOC_NULL_POINTER_ERROR(tempLevel->path3);
        tempLevel->towers = malloc(sizeof(Tower));
        MALLOC_NULL_POINTER_ERROR(tempLevel->towers);
        lineNumber = -1;
        mapOver = 0;
        towersOver = 0;

        char buffer[115];

        while(fgets(buffer, 114, levelFile)) {
            Tower tempTower;
            lineNumber++;
            if(buffer[0] == '-') {
                mapOver = 1; //the map has been parsed
                if(tempLevel->path1Length != 0) {
                    orderPath(tempLevel->path1, tempLevel->path1Length);
                }
                if(tempLevel->path2Length != 0) {
                    orderPath(tempLevel->path2, tempLevel->path2Length);
                }
                if(tempLevel->path3Length != 0) {
                    orderPath(tempLevel->path3, tempLevel->path3Length);
                }
            } else if (buffer[0] == '+') {
                towersOver = 1; //the towers have been parsed
            } else if (mapOver == 1 && towersOver == 0) {
                tempTower = parseTower(buffer, tempLevel); //create the tower
                tempLevel->towers = realloc(tempLevel->towers, sizeof(Tower)*(tempLevel->towersLength+1));
                MALLOC_NULL_POINTER_ERROR(tempLevel->towers);
                tempLevel->towers[tempLevel->towersLength] = tempTower; //adds the tower to the list of towers
                tempLevel->towersLength++; //increase the number of towers
            } else if (mapOver == 1 && towersOver == 1) { //get the money and win conditions
                char tempString[25];
                switch(buffer[0]) {
                    case 'm': //money
                        strcpy(tempString, &buffer[7]);
                        tempLevel->money = atoi(tempString);
                        break;
                    case 'w': //win condition
                        strcpy(tempString, &buffer[15]);
                        tempLevel->minionsToWin = atoi(tempString);
                        break;
                }
            } else {
                parseMap(buffer, lineNumber, tempLevel);
            }
        }

        levels[a-1] = *tempLevel; //add the level struct to the array
        fclose(levelFile); //close the current file
    }

    return levels;
}

/*
this function is only used to compare the positions of two points to make sure they are "adjacent"
this function is specifically called by orderPath
*/
static int checkAdjacentOrder(Point p1, Point p2) {
    // ORIGINALLY THIS WAS IN AN IF AS: ((path[d-1].pos.y == path[d].pos.y + 1) && (path[d-1].pos.x == path[d].pos.x)) || ((path[d-1].pos.y == path[d].pos.y - 1) && (path[d-1].pos.x == path[d].pos.x)) || ((path[d-1].pos.y == path[d].pos.y) && (path[d-1].pos.x == path[d].pos.x - 1))
    return ((p1.y == p2.y + 1) && (p1.x == p2.x)) || ((p1.y == p2.y - 1) && (p1.x == p2.x)) || ((p1.y == p2.y) && (p1.x == p2.x - 1));
}

/*
This function is used to make sure that the paths are actually in order
This function will only called after parsing a line
*/
static void orderPath(Point * path, int pathLength) {
    Point * tempPath = malloc(sizeof(Point)*pathLength);
    MALLOC_NULL_POINTER_ERROR(tempPath);

    int itemFound = 0;
    
    tempPath[0] = path[0]; //the first point in the path will be the beginning
    int tempPathLength = 1;

    while(tempPathLength != pathLength) { //cycle through the path until every element is reordered
        for (int d = 0; d < pathLength; d++) { //search through the path once to find the element that is next in the path
            itemFound = 0;
            /*the if is true if the point is directly: above, below, or to the right of the point before it*/
            if (checkAdjacentOrder(tempPath[tempPathLength-1], path[d])) {
                for (int i = 0; i < tempPathLength; i++) {
                    if (compare(tempPath[i], path[d])) {
                        itemFound = 1;
                    }
                }
                if (!itemFound) {
                    tempPath[tempPathLength] = path[d];
                    tempPathLength++;
                    break;
                }
            }
        }
    }

    for (int j = 0; j < pathLength; j++) { //setting path = tempPath
        path[j] = tempPath[j];
    }

    free(tempPath);
}

/*
This function interprets map data from a line of text
*/
static void parseMap(char * line, int lineNumber, Level * level) {
    Point tempPoint;
    tempPoint.y = lineNumber;

    for (int b = 0; b < strlen(line); b++) {
        tempPoint.x = b;
        switch(line[b]) {
            case 'a':
                shiftPoints(level->path1, level->path1Length); //make a spot for the beginning to be in position 1
                level->path1[0] = tempPoint;
                level->path1Length++;
                
                break;
            case '1':
                level->path1 = realloc(level->path1, sizeof(Point)*(level->path1Length+1)); //this gets done by shiftPoints but I'd rather have a redundant
                                                                             //function once than another random check with a flag or more duplicate code
                MALLOC_NULL_POINTER_ERROR(level->path1);
                level->path1[level->path1Length] = tempPoint;
                level->path1Length++;
                
                break;
            case 'b':
                shiftPoints(level->path2, level->path2Length); //make a spot for the beginning to be in position 1
                level->path2[0] = tempPoint;
                level->path2Length++;
                
                break;
            case '2':
                level->path2 = realloc(level->path2, sizeof(Point)*(level->path2Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path2);
                level->path2[level->path2Length] = tempPoint;
                level->path2Length++;
                
                break;
            case 'c':
                shiftPoints(level->path3, level->path3Length); //make a spot for the beginning to be in position 1
                level->path3[0] = tempPoint;
                level->path3Length++;
                
                break;
            case '3':
                level->path3 = realloc(level->path3, sizeof(Point)*(level->path3Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path3);
                level->path3[level->path3Length] = tempPoint;
                level->path3Length++;
                
                break;
            case '4':
                level->path1 = realloc(level->path1, sizeof(Point)*(level->path1Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path1);
                level->path1[level->path1Length] = tempPoint;
                level->path1Length++;
                
                level->path2 = realloc(level->path2, sizeof(Point)*(level->path2Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path2);
                level->path2[level->path2Length] = tempPoint;
                level->path2Length++;
                
                break;
            case '5':
                level->path1 = realloc(level->path1, sizeof(Point)*(level->path1Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path1);
                level->path1[level->path1Length] = tempPoint;
                level->path1Length++;
                
                level->path3 = realloc(level->path3, sizeof(Point)*(level->path3Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path3);
                level->path3[level->path3Length] = tempPoint;
                level->path3Length++;
                
                break;
            case '6':
                level->path2 = realloc(level->path2, sizeof(Point)*(level->path2Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path2);
                level->path2[level->path2Length] = tempPoint;
                level->path2Length++;
                
                level->path3 = realloc(level->path3, sizeof(Point)*(level->path3Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path3);
                level->path3[level->path3Length] = tempPoint;
                level->path3Length++;
                
                break;
            case 'x': //falls through so that every path gets this point
                level->exit = tempPoint;
            case '7':
                level->path1 = realloc(level->path1, sizeof(Point)*(level->path1Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path1);
                level->path1[level->path1Length] = tempPoint;
                level->path1Length++;
                
                level->path2 = realloc(level->path2, sizeof(Point)*(level->path2Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path2);
                level->path2[level->path2Length] = tempPoint;
                level->path2Length++;
                
                level->path3 = realloc(level->path3, sizeof(Point)*(level->path3Length+1));
                MALLOC_NULL_POINTER_ERROR(level->path3);
                level->path3[level->path3Length] = tempPoint;
                level->path3Length++;
                
                break;
        }
    }
}

/*
This function interprets towers from a line of text
*/
static Tower parseTower(char * line, Level * level) {
    char * token;
    char * typeString;
    Type newType;

    Point newPos;
    int y;
    int x;
    Tower newTower;

    /*get the tower type as a string*/
    typeString = strtok(line, " ");
    /*get the towers y and x position*/
    token = strtok(NULL, ",");
    y = atoi(token);
    token = strtok(NULL, ",");
    x = atoi(token);

    /*assign the tower its type*/
    if (strcmp(typeString, "bullet") == 0) {
        newType = bullet;
    } else if (strcmp(typeString, "explosive") == 0){
        newType = explosive;
    } else if (strcmp(typeString, "poison") == 0){
        newType = poison;
    } else if (strcmp(typeString, "freezing") == 0){
        newType = freezing;
    } else {
        newType = bullet;
    }

    /*get the tower position*/
    newPos = newPoint(x,y);

    newTower = createTower(newType, newPos);
    assignRange(&newTower, level->path1,level->path2,level->path3,level->path1Length,level->path2Length,level->path3Length);

    return newTower;
}

static void shiftPoints(Point * path, int pathLength) {
    if (pathLength > 0) {
        path = realloc(path, sizeof(Point)*(pathLength+1));
        MALLOC_NULL_POINTER_ERROR(path);
        for (int c = pathLength-1; c > -1; c--) {
            path[c+1] = path[c];
        }
    }
}
