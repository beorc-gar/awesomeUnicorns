#include "minion.h"

extern Minion createMinion(MinionType type, Point * path, int pathLength) {
    Minion minion;
    int i = 0;
    Colour colourF = white;
    Colour colourB = white;

    minion.path = path;
    minion.debuff = 0;
    minion.pos = 0;
    minion.type = type;
    minion.pathLength = pathLength;
    minion.bColour   = malloc(sizeof(int) *8);
    minion.fColour   = malloc(sizeof(int) *8);

    switch(type) {
        case speed:
            minion.health   = SPEED_HEALTH;
            minion.speed    = SPEED_SPEED;
            minion.maxSpeed = SPEED_SPEED;
            /*minion.cost     = SPEED_COST;*/
            minion.image    = SPEED_IMAGE;
            colourF         = yellow;
            break;
        case normal:
            minion.health   = NORMAL_HEALTH;
            minion.speed    = NORMAL_SPEED;
            minion.maxSpeed = NORMAL_SPEED;
            /*minion.cost     = NORMAL_COST;*/
            minion.image    = NORMAL_IMAGE;
            colourF         = black;
            break;
        case tank:
            minion.health   = TANK_HEALTH;
            minion.speed    = TANK_SPEED;
            minion.maxSpeed = TANK_SPEED;
            /*minion.cost     = TANK_COST;*/
            minion.image    = TANK_IMAGE;
            colourF         = magenta;
            break;
    }
    for(i = 0; i < 8; i++) {
        minion.bColour[i] = colourB;
        minion.fColour[i] = colourF;
    }

    return minion;
}

extern void moveMinions(Minion * minions, int numMinions) {
    int i;

    for(i = 0; i < numMinions; i++) {
        if(minions[i].health > 0 && !minionReachExit(minions[i])) {
            updateMinionSpeed(&minions[i]);
            minions[i].pos += minions[i].speed;
            if(minions[i].pos >= minions[i].pathLength) { /*to keep minion position in bounds*/
                minions[i].pos = minions[i].pathLength - 1;
            }
        }
    }
    return;
}

extern void updateMinionSpeed(Minion * minion) {
    if (minion->speed < minion->maxSpeed) {
        minion->speed++;
    }
    return;
}

extern int minionReachExit(Minion  minion) {
    return (minion.pos == minion.pathLength-1);
}

extern void destroyMinion(Minion* minion) {
    free(minion->fColour);
    free(minion->bColour);
    return;
}
