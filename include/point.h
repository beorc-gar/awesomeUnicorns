#ifndef POINT_H
#define POINT_H

#include "structs.h"

void copyPoint(Point* p1, Point p2);
Point newPoint(int x, int y);
/* Compare the two points to see if they are equal.
 * Params: Two points. */
int compare(Point p1, Point p2);
int compareRange(Point p, Point t, int range);

#endif
