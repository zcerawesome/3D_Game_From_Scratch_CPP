#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include "matrice.h"

GLfloat abs_to_float_x(int input)
{
    input -= 1920/2;
    float value = input / 960.0;
    return value;
}

GLfloat abs_to_float_y(int input)
{
    float value = input / 540.0;
    value += -1;
    return value;
}

std::vector<std::vector<std::vector<GLfloat>>> square = {{{100,100,100}, {100, 50, 100}, {50, 50, 100}, {50, 100, 100}}, 
                                                        {{100, 100, 100}, {100, 100, 50}, {100, 50, 50}, {100,50,100}},
                                                        {{50,50,100}, {100,50,100}, {100, 50, 50}, {50, 50, 50}},
                                                        {{50, 100, 100}, {50, 100, 50}, {50, 50, 50}, {50, 50, 100}},
                                                        {{100,100,100}, {100,100,50}, {50, 100, 50}, {50, 100, 100}},
                                                        {{50,50,50}, {50,100,50}, {100,100,50}, {100,50,50}}};

float d = 500;

float degree = 0.1;

float calc_x(std::vector<GLfloat>& plane)
{
    return plane[0] / (1 + plane[2]/d);
}

float calc_y(std::vector<GLfloat>& plane)
{
    // std::cout << plane[1] / (1 + plane[2]/d) << std::endl;
    return plane[1] / (1 + plane[2]/d);
}

void draw_line(std::vector<GLfloat>& plane1, std::vector<GLfloat>& plane2)
{
    glVertex2f(abs_to_float_x(calc_x(plane1)), abs_to_float_y(calc_y(plane1)));
    glVertex2f(abs_to_float_x(calc_x(plane2)), abs_to_float_y(calc_y(plane2)));
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    // Begin drawing a polygon (in this case, a square)
    glBegin(GL_LINES);  
    float l = 0;
    float r = 1920;
    float b = 0;
    float t = 1080;
    float n = 0.1;
    float f = 500;
    matrice<float> projection(4,4);
    projection.matrix = {{2/(r-l),0,0, -(r+l)/(r-l)}, {0, 2 /(t-b), 0, -(t+b)/(t-b)}, {0,0,-2/(f-n), -(f+n)/(f-n)}, {0,0,0,1}};
    matrice<float> x_rotation(3,3);
    x_rotation.matrix = {{1,0,0}, {0, cos(degree), -sin(degree)}, {0, sin(degree), cos(degree)}};
    for(std::vector<std::vector<GLfloat>>& squares: square)
    {
        
    }
    // degree += 0.1/2;
    glEnd();
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

int main()
{
    bool testing = 0;
    if(testing)
    {
        matrice<int> val(2,2);
        val.matrix = {{0,1}, {2,3}};
        matrice<int> second(2, 4);
        second.matrix = {{0,1, 2, 3}, {0,1, 2, 3}};
        matrice<int> test(2, 2);
        (val * second).toString();
        matrice<int> test2(2,3);
        std::vector<std::vector<int>> temp_ve = {{3,4,5},{0,1,2}};
        test2 = temp_ve;
        test2.toString();
        return 0;
    }
    for(std::vector<std::vector<GLfloat>>& squares: square)
    {
        for(std::vector<GLfloat>& plane: squares)
        {
            plane[0] += 960;
            plane[1] += 540;
        }
    }
    int argc = 1;
    char* argv[] = {(char*)"my_program"};
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
