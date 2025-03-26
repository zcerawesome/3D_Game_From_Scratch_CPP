#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include "matrice.h"
#include "Object.h"

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

void master_rotation_matrix(matrice<float>& rotation, float x, float y, float z)
{
    matrice<float> x_rotation(3,3);
    matrice<float> y_rotation(3,3);
    matrice<float> z_rotation(3,3);
    x_rotation.matrix = {{1,0,0}, {0, cos(x), -sin(x)}, {0, sin(x), cos(x)}};
    y_rotation.matrix = {{cos(y), 0, sin(y)}, {0, 1, 0}, {-sin(y), 0, cos(y)}};
    z_rotation.matrix = {{cos(z), -sin(z), 0}, {sin(z), cos(z), 0}, {0, 0, 1}};
    rotation.matrix = (x_rotation * y_rotation * z_rotation).matrix;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    // Begin drawing a polygon (in this case, a square)
    glBegin(GL_LINES);  
    float fov = 3.14 / 2.0;
    float aspect = MAX_WIDTH / MAX_HEIGHT;
    float zNear = 0.1f;
    float zFar = 500.0f;
    float f = 1.0f / tan(fov / 2.0f);
    matrice<float> projection(4,4);
    projection.matrix = {{f/aspect, 0,0,0}, {0, f,0,0}, {0,0,-(zFar + zNear) / (zNear-zFar), -(2*zFar*zNear) / (zNear-zFar)}, {0,0,1,0}};
    for(std::vector<std::vector<GLfloat>>& squares: square)
    {
        matrice<float> temp(4,1);
        temp = squares[0];
        temp[3][0] = 1.0;
        temp.matrix = (projection * temp).matrix;
        float x = (temp[0][0] / temp[3][0]) / 2 * MAX_WIDTH;
        float y = temp[1][0] / temp[3][0] / 2 * MAX_HEIGHT;
        std::cout << x << ", " << y << std::endl;
        temp = squares[1];
        temp[3][0] = 1.0;
        temp.matrix = (projection * temp).matrix;
        float x1 = temp[0][0] / temp[3][0] /2 * MAX_WIDTH;
        float y1 = temp[1][0] / temp[3][0] / 2 * MAX_HEIGHT;
        std::cout << x1 << ", " << y1 << std::endl;
        temp = squares[2];
        temp[3][0] = 1.0;
        temp.matrix = (projection * temp).matrix;
        float x2 = temp[0][0] / temp[3][0] / 2 * MAX_WIDTH;
        float y2 = temp[1][0] / temp[3][0] / 2 * MAX_HEIGHT;
        std::cout << x2 << ", " << y2 << std::endl;
        temp = squares[3];
        temp[3][0] = 1.0;
        temp.matrix = (projection * temp).matrix;
        float x3 = temp[0][0] / temp[3][0] / 2 * MAX_WIDTH;
        float y3 = temp[1][0] / temp[3][0] / 2 * MAX_HEIGHT;
        std::cout << x3 << ", " << y3 << std::endl;
        glVertex2f(abs_to_float_x(x),abs_to_float_y(y)); glVertex2f(abs_to_float_x(x1),abs_to_float_y(y1));
        glVertex2f(abs_to_float_x(x1),abs_to_float_y(y1)); glVertex2f(abs_to_float_x(x2),abs_to_float_y(y2));
        glVertex2f(abs_to_float_x(x2),abs_to_float_y(y2)); glVertex2f(abs_to_float_x(x3),abs_to_float_y(y3));
        glVertex2f(abs_to_float_x(x3),abs_to_float_y(y3)); glVertex2f(abs_to_float_x(x),abs_to_float_y(y));
    }
    // degree += 0.1/2;
    glEnd();
    exit(0);
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
        // (val * second).toString();
        matrice<int> test2(2,3);
        std::vector<std::vector<int>> temp_ve = {{3,4,5},{0,1,2}};
        test2 = temp_ve;
        // test2.toString();
        return 0;
    }
    for(std::vector<std::vector<GLfloat>>& squares: square)
    {
        for(std::vector<GLfloat>& plane: squares)
        {
            // plane[0] += 960;
            // plane[1] += 540;
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
