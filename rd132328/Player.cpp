//Player.cpp
#include "Player.hpp"
#include <cmath>
#include <cstdlib>
#include <GLFW/glfw3.h>

Player::Player(Level* level) 
    : level(level), 
      onGround(false), 
      yRot(0), 
      xRot(0), 
      xd(0), 
      yd(0), 
      zd(0),
      heightOffset(1.62f) {
    
    bb = AABB(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    resetPos();
}

void Player::resetPos() {
    float x = (float)(rand() % level->width);
    float y = (float)(level->depth + 10);
    float z = (float)(rand() % level->height);
    setPos(x, y, z);
}

void Player::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    float w = 0.3f;
    float h = 0.9f;
    bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void Player::turn(float xo, float yo) {
    yRot += xo * 0.15f;
    xRot -= yo * 0.15f;
    if (xRot < -90.0f) xRot = -90.0f;
    if (xRot > 90.0f) xRot = 90.0f;
}

void Player::tick() {
    xo = x;
    yo = y;
    zo = z;
    
    float xa = 0.0f, ya = 0.0f;
    
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R) == GLFW_PRESS) {
        resetPos();
    }
    
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS || 
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS) ya -= 1.0f;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS || 
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS) ya += 1.0f;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS || 
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS) xa -= 1.0f;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS || 
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS) xa += 1.0f;
    
    if ((glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) && onGround) {
        yd = 0.12f;
    }
    
    moveRelative(xa, ya, onGround ? 0.02f : 0.005f);
    yd -= 0.005f;
    move(xd, yd, zd);
    
    xd *= 0.91f;
    yd *= 0.98f;
    zd *= 0.91f;
    
    if (onGround) {
        xd *= 0.8f;
        zd *= 0.8f;
    }
}

void Player::move(float xa, float ya, float za) {
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;

    std::vector<AABB> aABBs = level->getCubes(bb.expand(xa, ya, za));
    
    for (auto& box : aABBs) {
        ya = box.clipYCollide(bb, ya);
    }
    bb.move(0.0f, ya, 0.0f);
    
    for (auto& box : aABBs) {
        xa = box.clipXCollide(bb, xa);
    }
    bb.move(xa, 0.0f, 0.0f);
    
    for (auto& box : aABBs) {
        za = box.clipZCollide(bb, za);
    }
    bb.move(0.0f, 0.0f, za);
    
    onGround = yaOrg != ya && yaOrg < 0.0f;
    
    if (xaOrg != xa) xd = 0.0f;
    if (yaOrg != ya) yd = 0.0f;
    if (zaOrg != za) zd = 0.0f;
    
    x = (bb.x0 + bb.x1) / 2.0f;
    y = bb.y0 + heightOffset;
    z = (bb.z0 + bb.z1) / 2.0f;
}

void Player::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (dist < 0.01f) return;
    
    dist = speed / sqrt(dist);
    xa *= dist;
    za *= dist;
    
    float sin = sinf(yRot * 3.14159f / 180.0f);
    float cos = cosf(yRot * 3.14159f / 180.0f);
    
    xd += xa * cos - za * sin;
    zd += za * cos + xa * sin;
}

void Player::setInputUp(bool pressed)    { keyUp = pressed; }
void Player::setInputDown(bool pressed)  { keyDown = pressed; }
void Player::setInputLeft(bool pressed)  { keyLeft = pressed; }
void Player::setInputRight(bool pressed) { keyRight = pressed; }
void Player::setInputJump(bool pressed)  { keyJump = pressed; }
void Player::setInputReset(bool pressed) { keyReset = pressed; }