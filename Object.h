#ifndef OBJECT_H
#define OBJECT_H

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

enum Shape {
    Circle,
    Rectangle,
};

struct Object
{
    Object(int x, int y, int z, int width, int height, Shape type): x(x), y(y), z(z), x_vel(0), y_vel(0), z_vel(0), width(width), height(height), Type(type)
    {
    }
    int x,y,z;
    float x_vel, y_vel, z_vel;
    int width, height;
    float r,g,b;
    Shape Type;
};

#endif