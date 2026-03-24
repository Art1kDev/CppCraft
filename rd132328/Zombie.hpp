//Zombie.hpp
#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Entity.hpp"
#include "Cube.hpp"

class Zombie : public Entity {
public:
    Cube head;
    Cube body;
    Cube arm0;
    Cube arm1;
    Cube leg0;
    Cube leg1;

    float rot;
    float timeOffs;
    float speed;
    float rotA;

    Zombie(Level* level, float x, float y, float z);

    void tick() override;
    void render(float a);
};

#endif