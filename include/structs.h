#ifndef STRUCT_H_BUCKLEY_DAVID
#define STRUCT_H_BUCKLEY_DAVID

typedef enum Colour {
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white
} Colour;

typedef enum Type {
    bullet,    /* Single target */
    freezing,
    explosive, /* AOE */
    poison
} Type;

typedef enum MinionType {
    normal,
	speed,
    tank
} MinionType;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Minion{
    /*Bronsons functional struct members	*/
    Point* path;
    int debuff;
    int health;
    int speed;
    int maxSpeed;

    /*Nisarg's edit*/
    MinionType type;

    /*Adams function struct members*/
    int pos;
    int pathLength;

    /*Graphics components*/
    char* image;
    int* fColour;
    int* bColour;
} Minion;

typedef struct Tower{
    /*Functional components*/
    Type type;

    Point pos;
    Point* range;

    int numPos; /* how many Points are in range */
    int damage;
    int reduction;
    int cooldown;
    int maxCooldown;

    /*Graphical components*/
    char * image;
    int * fColour;
    int * bColour;
} Tower;

typedef struct Level{
    Point* path1;
    Point* path2;
    Point* path3;

    Tower* towers;

    Point exit;

    int path1Length;
    int path2Length;
    int path3Length;

    int towersLength;
    int money;
    int minionsToWin;
} Level;
#endif
