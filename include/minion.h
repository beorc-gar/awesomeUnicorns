#ifndef MINION_H_OYEWUMI_BABALAKIN
#define MINION_H_OYEWUMI_BABALAKIN

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

#define SPEED_HEALTH 8
#define SPEED_SPEED 5
#define SPEED_COST 15
#define SPEED_IMAGE "_--> /\\ "

#define NORMAL_HEALTH 10
#define NORMAL_SPEED 3
#define NORMAL_COST 10
#define NORMAL_IMAGE "/{}D || "

#define TANK_HEALTH 20
#define TANK_SPEED 2
#define TANK_COST 20
#define TANK_IMAGE "|##O '' "

#define MAXMINIONS 100

/*Initializes all the the lements of the minion struct
 * Params: the type of minion, the path location, the path length*/
extern Minion createMinion(MinionType type, Point * path, int pathLength);

/*Moves the minions towards the exit
 *Params: the list of all minions, the amount of minions, the lenght of the path*/
extern void moveMinions(Minion * minions, int numMinions);

/*Increases the speed of the minion until it is at maxSpeed, this makes it keep moving even when it has been frozen
 *Params: the nth minion*/
extern void updateMinionSpeed(Minion * minion);

/*Checks if the minion has reached the exit yet
 *Params: the nth minion, the level information
 *Returns: True if minion has reached exit, false other wise*/
extern int minionReachExit(Minion  minion);

/* Frees memory associated with the minions EXCLUDING path.
 * Params: the list of all minions, and the number of minions in the list */
extern void destroyMinion(Minion* minions);

#endif
