#include "point.h"


void copyPoint(Point* p1, Point p2) {
    p1->x = p2.x;
    p1->y = p2.y;
}

Point newPoint(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    
    return p;
}

int compare(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}
int compareRange(Point p, Point t, int range) {
	int inX = (p.x >= t.x - range) && (p.x <= t.x + range);
	int inY = (p.y >= t.y - range) && (p.y <= t.y + range);
	return inX && inY;
}
