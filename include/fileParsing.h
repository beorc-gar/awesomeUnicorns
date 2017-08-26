#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tower.h"
#include "structs.h"
#include "point.h"

#ifndef FILEPARSING_H_STEER_LUCAS
#define FILEPARSING_H_STEER_LUCAS

#define NUMBER_OF_LEVELS 4
#define LEVEL_FILE_PREFIX "data/levelFiles/level"
#define SAVE_FILE "data/saveFile/saveFile.txt"
#define MALLOC_NULL_POINTER_ERROR(x) if(x == NULL) {printf("SYSTEM OUT OF MEMORY, CRITICAL ERROR, QUITING NOW.\n"); exit(1);}

/*NOTE, YOU MUST FREE THE RANGE FOR EACH TOWER AS THEY ARE NOT FREED WITHIN FILEPARSING.C*/
/*NOTE, RANGE CAN BE FREED WITH DESTROYTOWER()*/
/*NOTE, YOU MUST FREE EACH OF THE THREE PATHS AS WELL AS THE LIST OF TOWERS*/

/*
this is the function to call when a user unlocks
a certain level in order to update the save file
*/
extern void unlockLevel(int level);

/*
this is the function to call in order to get the
currently unlocked levels
*/
extern int getLevel(void);

/*
call to reset the highest currently unlocked level to 1
*/
extern void resetLevel(void);

/*
Call this function to load every level file into an array of Level structs
Returns a pointer to an array of Level structs
Function caller is responsible for freeing this
*/
extern Level * loadLevels(void);

#endif
