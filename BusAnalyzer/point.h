#ifndef POINT_H
#define POINT_H

class Point {
    private:
        float x;
        float y;
        float z;

    public:
        // constructor
        Point(float init_x, float init_y, float init_z);
        float getX();
        float getY();
        float getZ();
        void setX(float newX);
        void setY(float newY);
        void setZ(float newZ);
};

#endif // POINT_H
