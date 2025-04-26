#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "matrice.h"
#include "Object.h"
#include "Entity.h"

#define view_rad(x) x * M_PI / 2160

struct pos
{
    float x,y,z;
};

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
std::vector<std::vector<std::vector<GLfloat>>> triangle({{{0,10,50}, {-5, -10,45}, {5,-10,45}},
                                                        {{0,10, 50}, {5, -10, 45}, {5, -10, 55}},
                                                        {{0,10,50}, {5, -10, 55}, {-5, -10, 55}}, 
                                                        {{0,10,50}, {-5,-10,55}, {-5,-10,45}},
                                                        {{-5, -10, 45}, {5, -10, 45}, {5, -10, 55}, {-5,-10, 55}}});
std::vector<std::vector<std::vector<GLfloat>>> sun;
std::vector<std::vector<std::vector<GLfloat>>> earth;
std::vector<std::vector<std::vector<GLfloat>>> moon;
std::vector<std::vector<std::vector<GLfloat>>> donut;
std::vector<std::vector<std::vector<GLfloat>>> donut1;
float FOV = M_PI / 3;
float zNear = 0.1;
float zFar = 1000.0;
float aspect = 16.0/9.0;

int initial[2] = {-1,-1};
int mouseX, mouseY;


Entity player(0,0,0,0,0,0);
Entity Sun(0,0,-40);
Entity Earth(10,0,-40);
Entity Moon(11.5, 0, -40);
Entity Donut(0, 0, -20);
bool move[5] = {0,0,0,0,0};
bool mouse_in = true;
void mouseEntry(int state)
{
    if(state == GLUT_ENTERED)
        mouse_in = true;
    else if(state == GLUT_LEFT)
        mouse_in = false;
}

void mouseMove(int x, int y)
{
    mouseX += x - initial[0];
    mouseY += y - initial[1];
    if(mouseY > 1060)
        mouseY = 1060;
    else if(mouseY < -1060)
        mouseY = -1060;
    if(initial[0] == -1)
    {
        mouseY = 0;
        mouseX = 0;
        initial[0] = MAX_WIDTH/2;
        initial[1] = MAX_HEIGHT/2;
    }
}
int counter = 0;
void draw_line(matrice<GLfloat>& plane1, matrice<GLfloat>& plane2)
{
    if(( plane1[0][0] > 1 && plane2[0][0] > 1) || (plane1[0][0] < -1 && plane2[0][0] < -1) ||
        (plane1[1][0] > 1 && plane2[1][0] > 1) || (plane1[1][0] < -1 && plane2[1][0] < -1) ||
        (plane1[2][0] > 1 && plane2[2][0] > 1) || (plane1[2][0] < -1 && plane2[2][0] < -1))
        return;
    glVertex2f(plane1[0][0], plane1[1][0]);
    glVertex2f(plane2[0][0], plane2[1][0]);
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

int circular_index(int rel, int max)
{
    if(rel < 0)
        return max + rel % max;
    if(rel < max) 
        return rel;
    if(rel >= max)
        return rel % max;
    return -1;
}

void render_3D(std::vector<std::vector<std::vector<float>>>& object)
{
    matrice<GLfloat> projection({{1.0f / (aspect * tan(FOV/2)), 0,0,0}, 
                                                                {0, 1.0f/(tan(FOV/2)), 0,0},
                                                                {0,0, (zFar+zNear)/(zNear-zFar), (2*zFar*zNear)/(zNear-zFar)},
                                                                {0,0,-1,0}});
    matrice<float> rotation(3,3);
    master_rotation_matrix(rotation, (mouseY) * M_PI / 2160, (mouseX) * M_PI / 2160, 0);
    matrice<float> player_pos({player[0], player[1], player[2]});
    for(auto& squares: object)
    {
        std::vector<matrice<GLfloat>> points;
        for(int i = 0; i < squares.size(); i++)
            points.push_back((std::vector<GLfloat>)squares[i]);
        for(matrice<GLfloat>& point: points)
        {
            point.matrix = (point - player_pos).matrix;
            point.matrix = (rotation * point).matrix;
        }
        for(int i = 0; i < points.size()+1; i++)
        {
            matrice<GLfloat>& p0 = points[circular_index(i - 1, points.size())];
            matrice<GLfloat>& p1 = points[circular_index(i, points.size())];
            matrice<GLfloat>& p2 = points[circular_index(i + 1, points.size())];
            if(p1[2][0] < 0 || (p0[2][0] < 0 && p1[2][0] < 0 && p2[2][0] < 0) || (p0[2][0] > 0 && p1[2][0] > 0 && p2[2][0] > 0))
                continue;
            if(p2[2][0] < 0 && p0[2][0] < 0)
            {
                matrice<GLfloat> temp = p1;
                matrice<GLfloat> point(3,1);
                point.matrix = (p0 - p1).matrix;
                float t = (-0.01 - p1[2][0])/point[2][0];
                point.matrix = (point * t).matrix;
                p1.matrix = (point + p1).matrix;

                if(i != points.size())
                {
                    point.matrix = (p2 - temp).matrix;
                    t = (-0.01 - temp[2][0])/point[2][0];
                    point.matrix = (point * t).matrix;
                    temp.matrix = (point + temp).matrix;
                    points.insert(points.begin() + i+1, temp);
                }
            }
            else if(p0[2][0] > 0 && p2[2][0] < 0)
            {
                matrice<GLfloat> point(3,1);
                point.matrix = (p2 - p1).matrix;
                float t = (-0.01 - p1[2][0])/point[2][0];
                point.matrix = (point * t).matrix;
                p1.matrix = (point + p1).matrix;
            }
            else if(p2[2][0] > 0 && p0[2][0] < 0)
            {
                matrice<GLfloat> point(3,1);
                point.matrix = (p0 - p1).matrix;
                float t = (-0.01 - p1[2][0])/point[2][0];
                point.matrix = (point * t).matrix;
                p1.matrix = (point + p1).matrix;
            }
        }
        for(int i = 0; i < points.size(); i++)
        {
            matrice<GLfloat>& point = points[i];
            point.addRow({1.0f});
            point.matrix = (projection * point).matrix;
            if(!point[3][0])
                continue;
            for(int j = 0; j < 3; j++)
                point[j][0] /= point[3][0];
        }
        for(int i = 0; i < points.size(); i++)
            draw_line(points[i], points[circular_index(i+1, points.size())]);
    }
    if(mouse_in)
        glutWarpPointer(MAX_WIDTH/2, MAX_HEIGHT/2);
    mouseX %= 2160*2;
    if(player[1] > 0)
    {
        player.y_vel -= player.y_acc;
        if(player[1] + player.y_vel <= 0)
        {
            player[1] = 0;
            player.y_vel = 0;
        }
    }
}

void un_update_pos(Entity& inp, std::vector<std::vector<std::vector<float>>>& shape)
{
    for(auto& faces: shape)
    {
        for(auto& vertex: faces)
        {
            vertex[0] -= inp[0];
            vertex[1] -= inp[1];
            vertex[2] -= inp[2];
        }
    }
}

void update_pos(Entity& inp, std::vector<std::vector<std::vector<float>>>& shape)
{
    for(auto& faces: shape)
    {
        for(auto& vertex: faces)
        {
            vertex[0] += inp[0];
            vertex[1] += inp[1];
            vertex[2] += inp[2];
        }
    }
}

float distance_for(Entity& first, Entity& second)
{
    pos dif = {first.x - second.x, first.y - second.y, first.z - second.z};
    return sqrt(dif.x * dif.x + dif.y * dif.y + dif.z * dif.z);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);  
    // Begin drawing a polygon (in this case, a square)
    float forward = 0;
    float right = 0;
    if(!move[0] || !move[1])
        forward = 0;
    if(!move[2] || !move[3])
        right = 0;
    if(move[0])
        forward = -.2;
    else if(move[1])
        forward = .2;
    if(move[2])
        right = .2;
    else if(move[3])
        right = -.2;
    if(move[4] && player[1] == 0)
        player.y_vel = 0.3;
    forward *= 2; right *= 2;
    player.z_vel = forward * cos(view_rad(mouseX))  + right * sin(view_rad(mouseX));
    player.x_vel = -forward * sin(view_rad(mouseX)) + right * cos(view_rad(mouseX));
    player.update();
    glColor3f(1.0f, 1.0f, 0.0f);
    Sun.update();
    update_pos(Sun, sun);
    // render_3D(sun);
    un_update_pos(Sun, sun);
    glColor3f(0.0f, 1.0f, 0.0f);
    float distance = distance_for(Earth, Sun);
    if(distance != 0)
    {
        float acceleration = 0;
        if(distance >= 5.75)
            acceleration = 24.67401 / (distance * distance);
        else
            acceleration = 706504779.0 * 4 / 3 * M_PI * distance * 6.67e-11;
        Earth.x_vel += acceleration * (Sun[0] - Earth[0]) / distance;
        Earth.y_vel += acceleration * (Sun[1] - Earth[1]) / distance;
        Earth.z_vel += acceleration * (Sun[2] - Earth[2]) / distance;
    }
    distance = distance_for(Earth, Moon);
    if(distance != 0)
    {
        float acceleration = 0;
        if(distance >= 5.75)
            acceleration = 2.467401 / (distance * distance);
        else
            acceleration = 706504779.0 * 4 / 3 * M_PI * distance * 6.67e-11;
        Moon.x_vel += acceleration * (Earth[0] - Moon[0]) / distance;
        Moon.y_vel += acceleration * (Earth[1] - Moon[1]) / distance;
        Moon.z_vel += acceleration * (Earth[2] - Moon[2]) / distance;
    }
    Earth.update();
    update_pos(Earth, earth);
    // render_3D(earth);
    un_update_pos(Earth, earth);
    glColor3f(1.0f, 1.0f, 1.0f);
    Moon.update();
    update_pos(Moon, moon);
    // render_3D(moon);
    un_update_pos(Moon, moon);
    // update_pos(Donut, donut);
    render_3D(donut);
    render_3D(donut1);
    // un_update_pos(Donut, donut);
    glEnd();
    // exit(0);
    glFlush();
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(7, timer, 0);
}


void handleSpecialKeyPressUp(int key, int x, int y)
{
    *(int*)move = 0;
    move[4] = 0;
}

void handleSpecialKeyPress(int key, int x, int y)
{
    *(int*)move = 0;
    move[4] = 0;
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
    if(key == ' ')
        move[4] = 0;
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
    if(key == ' ')
        move[4] = 1;

}

void read_file(std::string file, std::vector<std::vector<std::vector<GLfloat>>>& object)
{
    std::ifstream file1(file);
    if(!file1.is_open())
        std::cerr << "Unable to open file" << std::endl;
    std::vector<std::vector<GLfloat>> Vertexes1;
    std::vector<std::vector<int>> Face1;
    std::string line1;
    while(std::getline(file1, line1))
    {
        std::stringstream ss(line1);
        std::string prefix;
        ss >> prefix;
        if(prefix == "v")
        {
            std::vector<GLfloat> temp(3);
            ss >> temp[0] >> temp[1] >> temp[2];
            Vertexes1.push_back(temp);
        }
        else if(prefix == "f")
        {
            std::string vData;
            std::vector<int> FaceInst;
            while(ss >> vData)
            {
                std::stringstream vss(vData);
                std::string value;
                int indices[3] = {0,0,0};
                for(int i =0; std::getline(vss, value, '/'); i++)
                    if(!value.empty()) indices[i] = std::stoi(value);
                FaceInst.push_back(indices[0] -1);
            }
            Face1.push_back(FaceInst);
        }
        else
            std::getline(file1, line1);
    }
    for(int i = 0; i < Face1.size(); i++)
    {
        std::vector<std::vector<GLfloat>>& v = Vertexes1;
        std::vector<std::vector<GLfloat>> temp_2D;
        for(int j =0; j < Face1[i].size(); j++)
        {
            temp_2D.push_back(v[Face1[i][j]]);    
        }
        object.push_back(temp_2D);
    }
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
        matrice<int> test2(2,3);
        std::vector<std::vector<int>> temp_ve = {{3,4,5},{0,1,2}};
        test2 = temp_ve;
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
    for(std::vector<std::vector<GLfloat>>& squares: triangle)
    {
        for(std::vector<GLfloat>& plane: squares)
        {
            plane[0] += 20;
            plane[2] *= -1;
        }
    }

    std::string input_file = "First_Cube.obj";
    input_file = "NewRatHead.obj";

    std::ifstream file1("Test.obj");
    if(!file1.is_open())
        std::cerr << "Unable to open file" << std::endl;
    std::vector<std::vector<GLfloat>> Vertexes1;
    std::vector<std::vector<int>> Face1;
    std::string line1;
    while(std::getline(file1, line1))
    {
        std::stringstream ss(line1);
        std::string prefix;
        ss >> prefix;
        if(prefix == "v")
        {
            std::vector<GLfloat> temp(3);
            ss >> temp[0] >> temp[1] >> temp[2];
            Vertexes1.push_back(temp);
        }
        else if(prefix == "f")
        {
            std::string vData;
            std::vector<int> FaceInst;
            while(ss >> vData)
            {
                std::stringstream vss(vData);
                std::string value;
                int indices[3] = {0,0,0};
                for(int i =0; std::getline(vss, value, '/'); i++)
                    if(!value.empty()) indices[i] = std::stoi(value);
                FaceInst.push_back(indices[0] -1);
            }
            Face1.push_back(FaceInst);
        }
        else
            std::getline(file1, line1);
    }
    for(int i = 0; i < Face1.size(); i++)
    {
        std::vector<std::vector<GLfloat>>& v = Vertexes1;
        std::vector<std::vector<GLfloat>> temp_2D;
        for(int j =0; j < Face1[i].size(); j++)
        {
            temp_2D.push_back(v[Face1[i][j]]);    
        }
        donut.push_back(temp_2D);
    }
    for(auto& squre1: donut)
        for(auto& row: squre1)
            row[2] += 10.0;
    std::ifstream file2("Donut2.obj");
    if(!file1.is_open())
        std::cerr << "Unable to open file" << std::endl;
    std::vector<std::vector<GLfloat>> Vertexes2;
    std::vector<std::vector<int>> Face2;
    std::string line2;
    while(std::getline(file2, line2))
    {
        std::stringstream ss(line2);
        std::string prefix;
        ss >> prefix;
        if(prefix == "v")
        {
            std::vector<GLfloat> temp(3);
            ss >> temp[0] >> temp[1] >> temp[2];
            Vertexes2.push_back(temp);
        }
        else if(prefix == "f")
        {
            std::string vData;
            std::vector<int> FaceInst;
            while(ss >> vData)
            {
                std::stringstream vss(vData);
                std::string value;
                int indices[3] = {0,0,0};
                for(int i =0; std::getline(vss, value, '/'); i++)
                    if(!value.empty()) indices[i] = std::stoi(value);
                FaceInst.push_back(indices[0] -1);
            }
            Face2.push_back(FaceInst);
        }
        else
            std::getline(file2, line2);
    }
    for(int i = 0; i < Face2.size(); i++)
    {
        std::vector<std::vector<GLfloat>>& v = Vertexes2;
        std::vector<std::vector<GLfloat>> temp_2D;
        for(int j =0; j < Face2[i].size(); j++)
        {
            temp_2D.push_back(v[Face2[i][j]]);    
        }
        donut1.push_back(temp_2D);
    }
    for(auto& squre1: donut1)
    {
        for(auto& row: squre1)
        {
            row[2] += 10.0;
            row[0] += 10.0;
        }

    }
    std::ifstream file("Sphere.obj");

    if(!file.is_open())
        std::cerr << "Unable to open file" << std::endl;

    std::vector<std::vector<GLfloat>> Vertexes;
    std::vector<std::vector<int>> Face;
    std::string line;
    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if(prefix == "v")
        {
            std::vector<GLfloat> temp(3);
            ss >> temp[0] >> temp[1] >> temp[2];
            Vertexes.push_back(temp);
        }
        else if(prefix == "f")
        {
            std::string vData;
            std::vector<int> FaceInst;
            while(ss >> vData)
            {
                std::stringstream vss(vData);
                std::string value;
                int indices[3] = {0,0,0};
                for(int i =0; std::getline(vss, value, '/'); i++)
                    if(!value.empty()) indices[i] = std::stoi(value);
                FaceInst.push_back(indices[0] -1);
            }
            Face.push_back(FaceInst);
        }
        else
            std::getline(file, line);
    }
    for(int i = 0; i < Face.size(); i++)
    {
        std::vector<std::vector<GLfloat>>& v = Vertexes;
        std::vector<std::vector<GLfloat>> temp_2D;
        for(int j =0; j < Face[i].size(); j++)
        {
            temp_2D.push_back(v[Face[i][j]]);    
        }
        sun.push_back(temp_2D);
    }
    earth = sun;
    moon = earth;
    for(auto& faces: earth)
    {
        for(auto& vertex: faces)
        {
            vertex[0] *= .07;
            vertex[1] *= .07;
            vertex[2] *= .07;
        }
    }
    for(auto& faces: moon)
    {
        for(auto& vertex: faces)
        {
            vertex[0] *= .02;
            vertex[1] *= .02;
            vertex[2] *= .02;
        }
    }
    Sun.x_vel = .1;
    Earth.z_vel = -1.57*1.2;
    Moon.z_vel = -1.28;
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
    glutSpecialFunc(handleSpecialKeyPress);
    glutSpecialUpFunc(handleSpecialKeyPressUp);
    glutPassiveMotionFunc(mouseMove);
    glutEntryFunc(mouseEntry);
    glutMotionFunc(mouseMove);
    glutMainLoop();
    return 0;
	
}
