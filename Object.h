#ifndef OBJECT_H
#define OBJECT_H

#define MAX_WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define MAX_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

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