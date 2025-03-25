#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "Object.h"
#include <math.h>
#include <vector>

std::vector<Object> entities = {{MAX_WIDTH/2,MAX_HEIGHT/2,0,8,8,Rectangle}, {MAX_WIDTH/2,78,0,8,8,Rectangle}};

float getX(int x)
{
    float value = x;
    value /= (1920.0/2);
    return value-1;
}
float getY(int y)
{
    float value = y;
    value /= (1080.0/2);
    return value-1;
}

float acceleration = .80;

Object& Sun = entities[0];
Object& Planet = entities[1];


float distance(Object& a, Object& b)
{
    float x_dis = a.x - b.x;
    float y_dis = a.y - b.y;
    return sqrt(x_dis * x_dis + y_dis * y_dis);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    // Begin drawing a polygon (in this case, a square)
    for(Object& entity: entities)
    {
        entity.x += entity.x_vel;
        entity.y += entity.y_vel;
        entity.z += entity.z_vel;
        if(entity.Type == Rectangle)
        {
            glColor3f(entity.r, entity.g, entity.b);
            glBegin(GL_QUADS);  
            glVertex2f(getX(entity.x - entity.width), getY(entity.y - entity.height));  // Bottom-left corner
            glVertex2f(getX(entity.x - entity.width), getY(entity.y + entity.height));  // Bottom-left corner
            glVertex2f(getX(entity.x + entity.width), getY(entity.y + entity.height));  // Bottom-left corner
            glVertex2f(getX(entity.x + entity.width), getY(entity.y - entity.height));  // Bottom-left corner
            glEnd();
        }
    }
    float dis = distance(entities[0], entities[1]);
    Planet.x_vel += acceleration * (Sun.x - entities[1].x) / dis;
    Planet.y_vel += acceleration * (Sun.y - entities[1].y) / dis;
    glFlush();
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(7, timer, 0);
}


void keyboards(unsigned char key, int x, int y)
{
    if(key == 27)
        exit(0);
}

void setColor(Object& obj, float r, float g, float b)
{
    obj.r = r;
    obj.g = g;
    obj.b = b;
}

int main()
{
    int argc = 1;
    char* argv[] = {(char*)"my_program"};
    setColor(entities[0], 1.0,1.0,0);
    setColor(entities[1], 1.0,0,0);
    Planet.x_vel = 40;
    // Planet.x_vel = 5;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode (single buffer, RGB color)
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT)); // Set window size (800x600 pixels)
    glutCreateWindow("Assembly Game"); // Create a window with a title
    // init();
    // Enter the GLUT event processing loop
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboards);
    glutMainLoop();
    return 0;
	
}
