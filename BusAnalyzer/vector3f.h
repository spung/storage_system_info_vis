#ifndef VECTOR3F_H
#define VECTOR3F_H
#include <QGLWidget>
#ifndef POINT
#define POINT
    #include "point.h"
#endif

class vector3f{
    public:
        vector3f(Point* pos, Point* vec);
        ~vector3f(){};
        void draw(float scale);
        float length();
        Point* getVector();
        void setVector(Point* vec);
        void setPosition(Point* pos);
        Point* getPosition();
        static float dotProduct(vector3f* a, vector3f* b);
        static vector3f* crossProduct(vector3f* a, vector3f* b);
        void normalize();
        void add(vector3f* adder);

    private:
        Point* position;
        Point* vector;
};


#endif // VECTOR3F_H
