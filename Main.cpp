#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include "matrice.h"
#include "Object.h"
#include "Entity.h"

#define view_rad(x) x * M_PI / 2160

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

std::vector<std::vector<std::vector<GLfloat>>> square({{{10,10,10}, {10, -10, 10}, {-10, -10, 10}, {-10, 10, 10}},
                                                        {{10,10,10}, {10, 10, -10}, {10, -10, -10}, {10,-10, 10}},
                                                        {{10,10,10}, {10,10,-10}, {-10, 10, -10}, {-10, 10, 10}}, 
                                                        {{-10, 10, 10}, {-10, 10, -10}, {-10, -10, -10}, {-10, -10, 10}},
                                                        {{-10, -10, 10}, {-10,-10,-10}, {10, -10, -10}, {10, -10, 10}},
                                                        {{-10,-10,-10}, {-10, 10, -10}, {10, 10, -10}, {10, -10, -10}}});

float FOV = M_PI / 3;
float zNear = 0.1;
float zFar = 1000.0;
float aspect = 16.0/9.0;

int initial[2] = {-1,-1};
int mouseX, mouseY;


Entity player(0,0,0,0,0,0);
bool move[4] = {0,0,0,0};

void mouseMove(int x, int y)
{
    mouseX += x - initial[0];
    mouseY += y - initial[1];
    if(initial[0] == -1)
    {
        mouseX = 0;
        mouseY = 0;
        initial[0] = MAX_WIDTH/2;
        initial[1] = MAX_HEIGHT/2;
    }
}

void its_clipping(matrice<GLfloat>& inside, matrice<GLfloat>& outside, float bound, short axis)
{
    float t = (bound - inside[axis][0]) / (outside[axis][0] - inside[axis][0]);
    for(int i = 0; i < 3; i++)
        outside[i][0]  = inside[i][0] + t * (outside[i][0] - inside[i][0]);
}

void draw_line(matrice<GLfloat>& plane1, matrice<GLfloat>& plane2)
{
    if(( plane1[0][0] > 1 && plane2[0][0] > 1) || (plane1[0][0] < -1 && plane2[0][0] < -1) ||
        (plane1[1][0] > 1 && plane2[1][0] > 1) || (plane1[1][0] < -1 && plane2[1][0] < -1) ||
        (plane1[2][0] > 1 && plane2[2][0] > 1) || (plane1[2][0] < -1 && plane2[2][0] < -1))
        return;
    if((abs(plane1[0][0]) <= 1) && (abs(plane1[1][0]) <= 1) && (abs(plane1[2][0]) <= 1) &&
        (abs(plane2[0][0]) <= 1) && (abs(plane2[1][0]) <= 1) && (abs(plane2[2][0]) <= 1))
    {
        glVertex2f(plane1[0][0], plane1[1][0]);
        glVertex2f(plane2[0][0], plane2[1][0]);
        return;
    }


    matrice<GLfloat>& p1 = plane1; 
    matrice<GLfloat>& p2 = plane2;
    bool p1Inside = (p1[0][0] >= -1 && p1[0][0] <= 1) && (p1[1][0] >= -1 && p1[1][0] <= 1) && (p1[2][0] >= -1 && p1[2][0] <= 1);
    bool p2Inside = (p2[0][0] >= -1 && p2[0][0] <= 1) && (p2[1][0] >= -1 && p2[1][0] <= 1) && (p2[2][0] >= -1 && p2[2][0] <= 1);

    // if(plane1[0][0] < -1) its_clipping(plane2, plane1, -1, 0);
    // if(plane1[0][0] > 1) its_clipping(plane2, plane1, 1, 0);
    // if(plane2[0][0] < -1) its_clipping(plane1, plane2, -1, 0);
    // if(plane2[0][0] > 1) its_clipping(plane1, plane2, 1, 0);

    // if(plane1[1][0] < -1) its_clipping(plane2, plane1, -1, 1);
    // if(plane1[1][0] > 1) its_clipping(plane2, plane1, 1, 1);
    // if(plane2[1][0] < -1) its_clipping(plane1, plane2, -1, 1);
    // if(plane2[1][0] > 1) its_clipping(plane1, plane2, 1, 1);

    // if(plane1[2][0] < -1) its_clipping(plane2, plane1, -1, 2);
    // if(plane1[2][0] > 1) its_clipping(plane2, plane1, 1, 2);
    // if(plane2[2][0] < -1) its_clipping(plane1, plane2, -1, 2);
    // if(plane2[2][0] > 1) its_clipping(plane1, plane2, 1, 2);
    // if(!p1Inside)
    // {
    //     if(p1[2][0] < -1) its_clipping(p2, p1, -1, 2);
    //     else if(p1[2][0] > 1) its_clipping(p2, p1, 1, 2);
    //     if(p1[0][0] < -1) its_clipping(p2, p1, -1, 0);
    //     else if(p1[0][0] > 1) its_clipping(p2, p1, 1, 0);
    //     if(p1[1][0] < -1) its_clipping(p2, p1, -1, 1);
    //     else if(p1[1][0] > 1) its_clipping(p2, p1, 1, 1);
    // }
    // if(!p2Inside)
    // {
    //     if(p2[2][0] < -1) its_clipping(p1, p2, -1, 2);
    //     else if(p2[2][0] > 1) its_clipping(p1, p2, 1, 2);
    //     if(p2[0][0] < -1) its_clipping(p1, p2, -1, 0);
    //     else if(p2[0][0] > 1) its_clipping(p1, p2, 1, 0);
    //     if(p2[1][0] < -1) its_clipping(p1, p2, -1, 1);
    //     else if(p2[1][0] > 1) its_clipping(p1, p2, 1, 1);
    // }
    // std::cout << "Plane 1 " << plane1[0][0] << ",\t" << plane1[1][0]  << " " << rand() << std::endl;
    // std::cout << "Plane 2 " << plane2[0][0] << ",\t" << plane2[1][0] << std::endl;
    if(plane1[2][0] < 0 || plane1[2][0] < 0)
        std::cout << rand() << std::endl;
    glVertex2f(plane1[0][0], plane1[1][0]);
    glVertex2f(plane2[0][0], plane2[1][0]);
    // std::cout << mouseX << std::endl;
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
    glBegin(GL_LINES);  
    // Begin drawing a polygon (in this case, a square)
    float forward = 0;
    float right = 0;
    if(move[0])
        forward = -.2;
    else if(move[1])
        forward = .2;
    if(move[2])
        right = .2;
    else if(move[3])
        right = -.2;
    
    player.z_vel += forward * cos(view_rad(mouseX))  + right * sin(view_rad(mouseX));
    player.x_vel += -forward * sin(view_rad(mouseX)) + right * cos(view_rad(mouseX));
    player.update();
    matrice<GLfloat> projection((std::vector<std::vector<float>>){{1.0f / (aspect * tan(FOV/2)), 0,0,0}, 
                                                                {0, 1.0f/(tan(FOV/2)), 0,0},
                                                                {0,0, (zFar+zNear)/(zNear-zFar), (2*zFar*zNear)/(zNear-zFar)},
                                                                {0,0,-1,0}});
    matrice<float> rotation(3,3);
    master_rotation_matrix(rotation, (mouseY) * M_PI / 2160, (mouseX) * M_PI / 2160, 0);
    for(std::vector<std::vector<GLfloat>>& squares: square)
    {
        std::vector<matrice<GLfloat>> points = {{matrice<GLfloat>(3,1)}, {matrice<GLfloat>(3,1)}, {matrice<GLfloat>(3,1)}, {matrice<GLfloat>(3,1)}};
        for(int i = 0; i < 4; i++)
            points[i] = squares[i];
        for(matrice<GLfloat>& point: points)
        {
            point[0][0] -= player[0];
            point[1][0] -= player[1];
            point[2][0] -= player[2];
            point.matrix = (rotation * point).matrix;
            point.addRow({1.0f});
            point[3][0] = 1.0f;
            point.matrix = (projection * point).matrix;
            if(!point[3][0])
                continue;
            for(int i = 0; i < 3; i++)
                point[i][0] /= point[3][0];
        }
        draw_line(points[0], points[1]);
        draw_line(points[1], points[2]);
        draw_line(points[2], points[3]);
        draw_line(points[3], points[0]);
    }
    if(mouseY > 1080)
        mouseY = 1080;
    else if(mouseY < -1080)
        mouseY = -1080;
    glutWarpPointer(MAX_WIDTH/2, MAX_HEIGHT/2);
    glEnd();
    // exit(0);
    glFlush();
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(7, timer, 0);
}



void keyboardUp(unsigned char key, int x, int y)
{
    if(key == 'w')
        move[0] = 0;
    if(key == 's')
        move[1] = 0;
    if(key == 'd')
        move[2] = 0;
    if(key == 'a')
        move[3] = 0;
}

void keyboards(unsigned char key, int x, int y)
{
    if(key == 27)
        exit(0);
    if(key == 'w')
        move[0] = 1;
    if(key == 's')
        move[1] = 1;

    if(key == 'd')
        move[2] = 1;
    if(key == 'a')
        move[3] = 1;
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
            plane[0] *= 5;
            plane[1] *= .5;
            plane[2] *= .5;
            plane[2] -= 50;
        }
    }
    int argc = 1;
    char* argv[] = {(char*)"my_program"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode (single buffer, RGB color)
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT)); // Set window size (800x600 pixels)
    glutCreateWindow("3D Game"); // Create a window with a title
    glutSetCursor(GLUT_CURSOR_NONE);
    
    // init();
    // Enter the GLUT event processing loop
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboards);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMove);
    glutMotionFunc(mouseMove);
    glutMainLoop();
    return 0;
	
}
