#include "point.h"

#ifndef POINT
#define POINT
    #include "point.h"
#endif

// Point represents a 3d point consisting of 3 float values
Point::Point(float init_x, float init_y, float init_z): x(init_x), y(init_y), z(init_z){}

float Point:: getX(){return x;}
float Point:: getY(){return y;}
float Point:: getZ(){return z;}

void Point:: setX(float newX){ x = newX; }
void Point:: setY(float newY){ y = newY; }
void Point:: setZ(float newZ){ z = newZ; }
