#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "Object.h"
#include <math.h>
#include <vector>

std::vector<Object> entities = {{MAX_WIDTH/2,MAX_HEIGHT/2,0,8,8,Rectangle}, {MAX_WIDTH/2,78,0,8,8,Rectangle}};

//Converts the x coordinate to the -1 to 1 range for opengl
float getX(int x)
{
    float value = x;
    value /= (1920.0/2);
    return value-1;
}
//Converts the y coordinate to the -1 to 1 range for opengl
float getY(int y)
{
    float value = y;
    value /= (1080.0/2);
    return value-1;
}

//Acceleration of the Planet
float acceleration = 0;

Object& Sun = entities[0];
Object& Planet = entities[1];

//Distance between two bodies
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
    for(Object& entity: entities)
    {
        //Update velocities of entity
        entity.x += entity.x_vel;
        entity.y += entity.y_vel;
        entity.z += entity.z_vel;
        //Draw rectangle and impose the correct color
        if(entity.Type == Rectangle)
        {
            glColor3f(entity.r, entity.g, entity.b);
            glBegin(GL_QUADS);  
            //Draw rectangle
            glVertex2f(getX(entity.x - entity.width), getY(entity.y - entity.height));  
            glVertex2f(getX(entity.x - entity.width), getY(entity.y + entity.height));  
            glVertex2f(getX(entity.x + entity.width), getY(entity.y + entity.height));  
            glVertex2f(getX(entity.x + entity.width), getY(entity.y - entity.height));  
            glEnd();
        }
    }
    //Find distance between the two orbitting bodies
    float dis = distance(entities[0], entities[1]);
    //Apply change in x and y velocity of the Planet
    Planet.x_vel += acceleration * (Sun.x - entities[1].x) / dis;
    Planet.y_vel += acceleration * (Sun.y - entities[1].y) / dis;
    //Update acceleration based on a = Gm1/(r^2)
    acceleration = 170755.2 / (dis * dis);
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
    //Put color of entities
    setColor(entities[0], 1.0,1.0,0);
    setColor(entities[1], 1.0,0,0);
    //Put Planet initial x velocity
    Planet.x_vel = 19.22;
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
