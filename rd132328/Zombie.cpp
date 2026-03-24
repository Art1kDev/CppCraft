//Zombie.cpp
#include "Zombie.hpp"
#include "Textures.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>

static float rnd01() {
    return (float)std::rand() / (float)RAND_MAX;
}

Zombie::Zombie(Level* level, float x_, float y_, float z_)
    : Entity(level),
      head(0, 0),
      body(16, 16),
      arm0(40, 16),
      arm1(40, 16),
      leg0(0, 16),
      leg1(0, 16),
      rot(0.0f),
      timeOffs(rnd01() * 1239813.0f),
      speed(1.0f),
      rotA((rnd01() + 1.0f) * 0.01f) {
    const float pi = 3.14159265358979323846f;
    rot = rnd01() * (pi * 2.0f);

    setPos(x_, y_, z_);

    head.addBox(-4.0f, -8.0f, -4.0f, 8, 8, 8);

    body.addBox(-4.0f, 0.0f, -2.0f, 8, 12, 4);

    arm0.addBox(-3.0f, -2.0f, -2.0f, 4, 12, 4);
    arm0.setPos(-5.0f, 2.0f, 0.0f);

    arm1.addBox(-1.0f, -2.0f, -2.0f, 4, 12, 4);
    arm1.setPos(5.0f, 2.0f, 0.0f);

    leg0.addBox(-2.0f, 0.0f, -2.0f, 4, 12, 4);
    leg0.setPos(-2.0f, 12.0f, 0.0f);

    leg1.addBox(-2.0f, 0.0f, -2.0f, 4, 12, 4);
    leg1.setPos(2.0f, 12.0f, 0.0f);
}

void Zombie::tick() {
    xo = x;
    yo = y;
    zo = z;

    rot += rotA;
    rotA *= 0.99f;
    rotA = (float)((double)rotA + ((rnd01() - rnd01()) * rnd01() * rnd01() * 0.01f));

    float xa = std::sin(rot);
    float za = std::cos(rot);

    if (onGround && rnd01() < 0.01f) {
        yd = 0.12f;
    }

    moveRelative(xa, za, onGround ? 0.02f : 0.005f);
    yd -= 0.005f;
    move(xd, yd, zd);

    xd *= 0.91f;
    yd *= 0.98f;
    zd *= 0.91f;

    if (y > 100.0f) {
        resetPos();
    }

    if (onGround) {
        xd *= 0.8f;
        zd *= 0.8f;
    }
}

void Zombie::render(float a) {
    glEnable(GL_TEXTURE_2D);
    GLuint tex = Textures::loadTexture("char.png", GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPushMatrix();

    double time = glfwGetTime() * 10.0 * (double)speed + (double)timeOffs;
    float size = 0.058333334f;
    float yy = (float)(-std::abs(std::sin(time * 0.6662)) * 5.0 - 23.0);

    float rx = xo + (x - xo) * a;
    float ry = yo + (y - yo) * a;
    float rz = zo + (z - zo) * a;

    glTranslatef(rx, ry, rz);
    glScalef(1.0f, -1.0f, 1.0f);
    glScalef(size, size, size);
    glTranslatef(0.0f, yy, 0.0f);

    float c = 57.29578f;
    glRotatef(rot * c + 180.0f, 0.0f, 1.0f, 0.0f);

    head.yRot = (float)std::sin(time * 0.83) * 1.0f;
    head.xRot = (float)std::sin(time) * 0.8f;

    const double pi = 3.14159265358979323846;

    arm0.xRot = (float)std::sin(time * 0.6662 + pi) * 2.0f;
    arm0.zRot = (float)(std::sin(time * 0.2312) + 1.0) * 1.0f;

    arm1.xRot = (float)std::sin(time * 0.6662) * 2.0f;
    arm1.zRot = (float)(std::sin(time * 0.2812) - 1.0) * 1.0f;

    leg0.xRot = (float)std::sin(time * 0.6662) * 1.4f;
    leg1.xRot = (float)std::sin(time * 0.6662 + pi) * 1.4f;

    head.render();
    body.render();
    arm0.render();
    arm1.render();
    leg0.render();
    leg1.render();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}