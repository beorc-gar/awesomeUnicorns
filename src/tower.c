#include "tower.h"

 Tower createTower(Type type, Point pos) {
    Tower tower;

    int i           = 0;
    Colour colourF  = white;
    Colour colourB  = white;

    tower.type      = type;
    tower.pos       = pos;
    tower.cooldown  = 0;
    tower.reduction = 0;
    tower.damage    = 0;
    tower.bColour   = malloc(sizeof(int) *8);
    tower.fColour   = malloc(sizeof(int) *8);
    tower.image     = "        ";

    switch(type) {
        case bullet:
            tower.damage      = BULLET_DAMAGE;
            tower.maxCooldown = BULLET_COOLDOWN;
            tower.image       = BULLET_IMAGE;
            colourF           = black;
            break;
        case explosive:
            tower.damage      = EXPLOSIVE_DAMAGE;
            tower.maxCooldown = EXPLOSIVE_COOLDOWN;
            tower.image       = EXPLOSIVE_IMAGE;
            colourF           = red;
            break;
        case poison:
            tower.damage      = POISON_DAMAGE;
            tower.maxCooldown = POISON_COOLDOWN;
            tower.image       = POISON_IMAGE;
            colourF           = green;
            break;
        case freezing:
            tower.maxCooldown = FREEZING_COOLDOWN;
            tower.reduction   = FREEZING_REDUCTION;
            tower.image       = FREEZING_IMAGE;
            colourF           = cyan;
            break;
    }
    for(i = 0; i < 8; i++) {
        tower.bColour[i] = colourB;
        tower.fColour[i] = colourF;
    }
    return tower;
}

 void assignRange(Tower* tower, Point* p1, Point* p2, Point* p3, int pl1, int pl2, int pl3) {
    int i           = 0;
    int j           = 0;
    int towerLength = 0;
    int range       = 0;
    int duplicate   = 0;
    Point* inRange  = malloc(sizeof(Point));

    switch(tower->type) {
        case bullet:
            range = BULLET_RANGE;
            break;
        case explosive:
            range = EXPLOSIVE_RANGE;
            break;
        case poison:
            range = POISON_RANGE;
            break;
        case freezing:
            range = FREEZING_RANGE;
            break;
        default:
            return;
    }

    for(i = 0; i < pl1; i++) {
        if(compareRange(p1[i], tower->pos, range)) {
            for(j = 0; j < towerLength; j++) {
                if(compare(p1[i], inRange[j])) {
                    duplicate = 1;
                }
            }
            if(!duplicate) {
                towerLength++;
                inRange = realloc(inRange, sizeof(Point)*towerLength);
                inRange[towerLength-1] = p1[i];
            }
        }
        duplicate = 0;
    }
    
    for(i = 0; i < pl2; i++) {
        if(compareRange(p2[i], tower->pos, range)) {
            for(j = 0; j < towerLength; j++) {
                if(compare(p2[i], inRange[j])) {
                    duplicate = 1;
                }
            }
            if(!duplicate) {
                towerLength++;
                inRange = realloc(inRange, sizeof(Point)*towerLength);
                inRange[towerLength-1] = p2[i];
            }
        }
        duplicate = 0;
    }

    for(i = 0; i < pl3; i++) {
        if(compareRange(p3[i], tower->pos, range)) {
            for(j = 0; j < towerLength; j++) {
                if(compare(p3[i], inRange[j])) {
                    duplicate = 1;
                }
            }
            if(!duplicate) {
                towerLength++;
                inRange = realloc(inRange, sizeof(Point)*towerLength);
                inRange[towerLength-1] = p3[i];
            }
        }
        duplicate = 0;
    }
    
    tower->range  = inRange;
    tower->numPos = towerLength;
    return;
}

/* Helper functions for found */
static Minion** find(Tower* tower, Minion* minions, int numMinions, int* numFound);
static Minion** findAll(Tower* tower, Minion* minions, int numMinions, int* numFound);
static Minion** findUnfrozen(Tower* tower, Minion* minions, int numMinions, int* numFound);

 Minion** found(Tower* tower, Minion* minions, int numMinions, int* numFound) {
    *numFound = 0;

    switch(tower->type) {
        case bullet:
            return find(tower, minions, numMinions, numFound);
        case explosive: /* fall through */
        case poison:
            return findAll(tower, minions, numMinions, numFound);
        case freezing:
            return findUnfrozen(tower, minions, numMinions, numFound);
    }
    return malloc(1);
}

/* Helper functions for damage */
static void shoot(Tower* tower, Minion** minion);
static void explode(Tower* tower, Minion** minions, int numMinions);
static void poisone(Tower* tower, Minion** minions, int numMinions);
static void freeze(Tower* tower, Minion** minion);
static void setCooldown(Tower* tower);
static void kill(Minion* minion);

 void damage(Tower* tower, Minion** minions, int numMinions) {
    tower->bColour[1] = tower->bColour[0];
    tower->bColour[2] = tower->bColour[0];

    if(tower->cooldown != 0 || minions == NULL || numMinions == 0) {
        return; /* don't deal damage if tower is waiting on cooldown. */
    }           /* or if no minions are in range */
    switch(tower->type) {
        case bullet:
            shoot(tower, minions);
            break;
        case explosive:
            explode(tower, minions, numMinions);
            break;
        case poison:
            poisone(tower, minions, numMinions);
            break;
        case freezing:
            freeze(tower, minions);
            break;
    }
    setCooldown(tower);
    tower->bColour[1] = yellow;
    tower->bColour[2] = yellow;

    return;
}

 void decrementCooldown(Tower* tower) {
    if(tower->cooldown > 0) {
        tower->cooldown--;
    } else {
        tower->cooldown = 0; /* just in case by some fluke */
    }
    return;
}

 void destroyTower(Tower* tower) {
    free(tower->bColour);
    free(tower->fColour);
    free(tower->range);
    return;
}

 void resetColour(Minion* minions, int numMinions) {
    int i = 0;
    int j = 0;

    for(i = 0; i < numMinions; i++) {
        for(j = 0; j < 8; j++) {
            minions[i].fColour[j] = minions[i].fColour[0];
        }
        if(minions[i].speed < minions[i].maxSpeed) {
            minions[i].fColour[5] = cyan;
            minions[i].fColour[6] = cyan;
        }
        if(minions[i].debuff > 1) {
            minions[i].fColour[3] = green;
        }
    }
    return;
}

 void updateDebuff(Level level, Minion* minions, int numMinions) {
    int i = 0;
    int j = 0;
    int old = 0;
    int numFound = 0;
    int numPoisoned = 0;
    int inPoison = 0;
    Minion** temp = NULL;
    Minion** poisoned = malloc(sizeof(Minion*));

    for(i = 0; i < level.towersLength; i++) {
        if(level.towers[i].type == poison) {
            numFound = 0;
            temp = found(&level.towers[i], minions, numMinions, &numFound);
            old = numPoisoned;
            numPoisoned += numFound;
            poisoned = realloc(poisoned, sizeof(Minion*)*numPoisoned);
            for(j = old; j < numPoisoned; j++) {
                poisoned[j] = temp[j-old];
            }
            free(temp);
        }
    }

    for(i = 0; i < numMinions; i++) {
        inPoison = 0;
        for(j = 0; j < numPoisoned; j++) {
            if(&minions[i] == poisoned[j]) {
                inPoison = 1;
                break;
            }
        }
        if(!inPoison) {
            minions[i].debuff = 1;
        }
    }
    free(poisoned);
    return;
}

/*------------------------------------Helper Functions-------------------------------------------*/

/* Finds the leading minion in this tower's range.
 * Params:  the tower, and the list of all minions.
 * Returns: a minion. */
static Minion** find(Tower* tower, Minion* minions, int numMinions, int* numFound) {
    Minion** inRange = malloc(sizeof(Minion*));
    int i   = 0;
    int j   = 0;
    int pos = 0;

    for(i = 0; i < numMinions; i++) {
        for(j = 0; j < tower->numPos; j++) {
            if(compare(minions[i].path[minions[i].pos], tower->range[j])) {
                if(minions[i].pos >= pos) {
                    *inRange = &minions[i];
                    pos = minions[i].pos;
                }
                *numFound = 1;
            }
        }
    }
    return inRange;
}

/* Finds all the minions in this tower's range.
 * Params:  the tower, and the list of all minions.
 * Returns: a list of minions. */
static Minion** findAll(Tower* tower, Minion* minions, int numMinions, int* numFound) {
    Minion** inRange = malloc(sizeof(Minion*));
    int i = 0;
    int j = 0;

    for(i = 0; i < numMinions; i++) {
        for(j = 0; j < tower->numPos; j++) {
            if(compare(minions[i].path[minions[i].pos], tower->range[j])) {
                inRange = realloc(inRange, sizeof(Minion*)*((*numFound)+1));
                inRange[*numFound] = &minions[i];
                *numFound = *numFound + 1;
                break;
            }
        }
    }
    return inRange;
}

/* Finds the first minion in this tower's range that isn't frozen.
 * Params:  the tower, and the list of all minions.
 * Returns: a minion. */
static Minion** findUnfrozen(Tower* tower, Minion* minions, int numMinions, int* numFound) {
    Minion** inRange  = malloc(sizeof(Minion*));
    Minion** unfrozen = malloc(sizeof(Minion*));
    int i             = 0;
    int j             = 0;
    int pos           = 0;
    int speedReduc    = 0;

    *unfrozen = NULL;

    for(i = 0; i < numMinions; i++) {
        for(j = 0; j < tower->numPos; j++) {
            if(compare(minions[i].path[minions[i].pos], tower->range[j])) {
                if(minions[i].pos >= pos) {
                    *inRange = &minions[i];
                    speedReduc = (*inRange)->maxSpeed - (*inRange)->speed;
                    pos = minions[i].pos;
                    if(speedReduc == 0) {
                        *unfrozen = *inRange;
                    }
                }
                *numFound = 1;
            }
        }
    }

    if(*unfrozen != NULL) {
        free(inRange);
        return unfrozen;
    }
    free(unfrozen);
    return inRange;
}

/* Should be called with the minion returned by find().
 * Damages the minion.
 * Params:  the tower, and a minion. */
static void shoot(Tower* tower, Minion** minion) {
    (*minion)->health -= tower->damage;
    (*minion)->fColour[2] = red;
    if((*minion)->health <= 0) {
        kill(*minion);
    }
    return;
}

/* Should be called with the minions returned by findAll().
 * Damages all the minions in the list.
 * Params:  the tower, and a list of minions. */
static void explode(Tower* tower, Minion** minions, int numMinions) {
    int i = 0;

    for(i = 0; i < numMinions; i++) {
    minions[i]->fColour[1] = red;
        shoot(tower, &minions[i]);
    }
    return;
}

/* Should be called with the minions returned by findAll().
 * Damages all the minions in the list and applies a debuff.
 * Params:  the tower, and a list of minions. */
static void poisone(Tower* tower, Minion** minions, int numMinions) {
    int i = 0;

    for(i = 0; i < numMinions; i++) {
        minions[i]->health -= tower->damage * minions[i]->debuff;
        minions[i]->debuff++;
        minions[i]->fColour[3] = green;
        if(minions[i]->health <= 0) {
            kill(minions[i]);
        }
    }
    return;
}

/* Should be called with the minions returned by findUnfrozen().
 * Slows down a minion.
 * Params:  the tower, and a list of minions. */
static void freeze(Tower* tower, Minion** minion) {
    (*minion)->speed = (*minion)->maxSpeed - tower->reduction;
    (*minion)->fColour[5] = cyan;
    (*minion)->fColour[6] = cyan;

    if((*minion)->speed < 1) {
        (*minion)->speed = 1;
    }
    return;
}

/* Sets this tower's cooldown to its maxCooldown.
 * Params:  the tower. */
static void setCooldown(Tower* tower) {
    tower->cooldown = tower->maxCooldown;
    return;
}

/* Sets the values of this minion so it isn't used.
 * Params:  the minion. */
static void kill(Minion* minion) {
    minion->health   = 0;
    minion->pos      = 0;
    minion->speed    = 0;
    minion->maxSpeed = 0;
    minion->debuff   = 0;
    return;
}
