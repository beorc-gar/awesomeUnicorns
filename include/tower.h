#ifndef TOWER_H_
#define TOWER_H_

#include <stdlib.h>
#include "structs.h"
#include "point.h"

/* fiddle with these numbers for game balance */
/* fiddle with these images for aesthetics */

#define BULLET_DAMAGE 1
#define BULLET_COOLDOWN 2
#define BULLET_IMAGE "|++| || "
#define BULLET_RANGE 15

#define EXPLOSIVE_DAMAGE 4
#define EXPLOSIVE_COOLDOWN 6
#define EXPLOSIVE_IMAGE "\\''/ /\\ "
#define EXPLOSIVE_RANGE 10

#define POISON_DAMAGE 2
#define POISON_COOLDOWN 5
#define POISON_IMAGE "[==] [] "
#define POISON_RANGE 10

#define FREEZING_REDUCTION 3
#define FREEZING_COOLDOWN 3
#define FREEZING_IMAGE "{::} II "
#define FREEZING_RANGE 10

/* Creates a tower.
 * Params: the type, and its position.
 * Returns: a tower with the desired members
 * members that must be free'd : bColour, fColour, range
 * NOTE: uses the definitions above to set damage, reduction, maxCooldown, and image */
Tower createTower(Type type, Point pos);

/* Assigns a range to the given tower.
 * Params: the tower, the three paths, and the 3 path lengths. */
void assignRange(Tower* tower, Point* p1, Point* p2, Point* p3, int pl1, int pl2, int pl3);

/* Calls the appropriate function to find minions with for this type of tower.
 * i.e. find for bullet, find all for explosive, etc.
 * Sets the value inside the integer pointer to how many minions are in range.
 * Params:  the tower, the list of all minions, the number of minions in the list,
 *          and a pointer to set as how many it found.
 * Returns: a list of pointers to minions.
 * NOTE: the returned Minion** must be free'd. */
Minion** found(Tower* tower, Minion* minions, int numMinions, int* numFound);

/* Should be called with the minions returned by found().
 * Calls the appropriate function to damage with for this type of tower.
 * i.e. shoot for bullet, explode for explosive, etc.
 * Params:  the tower, a list of minions, and how many minions are in the list.
 *          note: this number should be the number that was set from found() */
void damage(Tower* tower, Minion** minions, int numMinions);

/* Decrements this tower's cooldown by 1.
 * Won't work if cooldown is 0.
 * Params:  the tower. */
void decrementCooldown(Tower* tower);

/* Frees all memory that was allocated in createTower() and 
 * values that are assumed to be malloced and passed in as parameters to createTower()
 * Params:  the tower. */
void destroyTower(Tower* tower);

/* Restores this minions foreground colour to it's priginal
 * Params: list of minions, and how many minions. */
void resetColour(Minion* minions, int numMinions);

/* Restores this minions debuff if they aren't targetted by a poison tower.
 * Params: the level, list of minions, and how many minions. */
void updateDebuff(Level level, Minion* minions, int numMinions);

#endif
