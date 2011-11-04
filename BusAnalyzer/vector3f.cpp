#include "vector3f.h"

#include <math.h>
#include <stdio.h>
vector3f::vector3f(Point* pos, Point* vec): position(pos), vector(vec){}

Point* vector3f:: getVector(){ return vector;}
Point* vector3f:: getPosition(){ return position;}

void vector3f:: setVector(Point* vec){
    delete(vector);
    vector = vec;
}

void vector3f:: setPosition(Point* pos){
    delete(position);
    position = pos;
}

void vector3f:: normalize(){
    float len = length();
    //printf("before x: %g\n",vector->getX());
    if(position->getX() != 0.0)
        printf("ERROR\n\n");
    vector->setX(vector->getX()/len);//printf("after x: %g\n",vector->getX());
    vector->setY(vector->getY()/len);
    vector->setZ(vector->getZ()/len);
}

void vector3f:: add(vector3f* adder){
    vector->setX(vector->getX() + adder->getVector()->getX());
    vector->setY(vector->getY() + adder->getVector()->getY());
    vector->setZ(vector->getZ() + adder->getVector()->getZ());
}

void vector3f:: draw(float scale){
    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 0.0);
            glVertex3f(position->getX(),position->getY(), position->getZ());
            glVertex3f(vector->getX()*scale + position->getX(),vector->getY()*scale + position->getY(), vector->getZ()*scale + position->getZ());
        glEnd();
}

float vector3f:: length(){
    float delX = 0.0 - vector->getX();
    float delY = 0.0 - vector->getY();
    float delZ = 0.0 - vector->getZ();

    return sqrtf(delX*delX + delY*delY + delZ*delZ);
}

float vector3f::dotProduct(vector3f* a, vector3f* b){
    Point* a_vec = a->getVector();
    Point* b_vec = b->getVector();
    return (float)a_vec->getX()*b_vec->getX() + (float)a_vec->getY()*b_vec->getY() + (float)a_vec->getZ()*b_vec->getZ();
}

vector3f* vector3f::crossProduct(vector3f* a, vector3f* b){
    Point* a_vec = a->getVector();
    Point* b_vec = b->getVector();

    float a_x = a_vec->getX();
    float a_y = a_vec->getY();
    float a_z = a_vec->getZ();

    float b_x = b_vec->getX();
    float b_y = b_vec->getY();
    float b_z = b_vec->getZ();

    return new vector3f(new Point(0.0, 0.0, 0.0), new Point(a_y*b_z - a_z*b_y, a_z*b_x - a_x*b_z, a_x*b_y - a_y*b_x));
}
