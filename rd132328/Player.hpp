//Player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AABB.hpp"
#include "Level.hpp"
#include <vector>

class Player {
private:
    Level* level;
    
public:
    float xo, yo, zo;
    float x, y, z;
    float xd, yd, zd;
    float yRot, xRot;
    AABB bb;
    bool onGround;
    float heightOffset;

    Player(Level* level);
    void resetPos();
    void setPos(float x, float y, float z);
    void turn(float xo, float yo);
    void tick();
    void move(float xa, float ya, float za);
    void moveRelative(float xa, float za, float speed);
    
    void setInputUp(bool pressed);
    void setInputDown(bool pressed);
    void setInputLeft(bool pressed);
    void setInputRight(bool pressed);
    void setInputJump(bool pressed);
    void setInputReset(bool pressed);
    
private:
    bool keyUp = false;
    bool keyDown = false;
    bool keyLeft = false;
    bool keyRight = false;
    bool keyJump = false;
    bool keyReset = false;
};

#endif