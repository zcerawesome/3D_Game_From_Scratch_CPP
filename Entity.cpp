#include "Entity.h"

Entity::Entity(float x, float y, float z): x(x), y(y), z(z)
{}

Entity::Entity(float x, float y, float z, float x_degree, float y_degree, float z_degree):x(x), y(y), z(z), x_degree(x_degree), y_degree(y_degree), z_degree(z_degree)
{}

void Entity::update()
{
    x += x_vel;
    y += y_vel;
    z += z_vel;
}

float& Entity::operator[](int index)
{
    return *(&x + index);
}