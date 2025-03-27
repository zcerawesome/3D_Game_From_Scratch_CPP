#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <iostream>
class Entity
{
public:
    float x,y,z;
    float x_degree, y_degree, z_degree;
    float x_vel = 0, y_vel=0, z_vel=0;
    std::vector<std::vector<std::vector<float>>> vertices;
    Entity(float x, float y, float z);
    Entity(float x, float y, float z, float x_degree, float y_degree, float z_degree);
    void update();
    float& operator[](int index);
};

#include "Entity.cpp"
#endif