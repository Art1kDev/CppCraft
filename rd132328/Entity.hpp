//Entity.hpp
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Level.hpp"
#include "AABB.hpp"

class Entity {
public:
    Level* level;

    float xo, yo, zo;
    float x, y, z;
    float xd, yd, zd;
    float yRot, xRot;

    AABB bb;
    bool onGround;
    float heightOffset;

    Entity(Level* level);
    virtual ~Entity() = default;

    virtual void tick();
    void resetPos();
    void setPos(float x, float y, float z);
    void turn(float xo, float yo);
    void move(float xa, float ya, float za);
    void moveRelative(float xa, float za, float speed);
};

#endif