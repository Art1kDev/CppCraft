#include "Entity.hpp"
#include <cmath>
#include <cstdlib>

Entity::Entity(Level* level)
    : level(level),
      xo(0), yo(0), zo(0),
      x(0), y(0), z(0),
      xd(0), yd(0), zd(0),
      yRot(0), xRot(0),
      onGround(false),
      heightOffset(0.0f),
      bb(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f) {
    resetPos();
}

void Entity::resetPos() {
    float px = (float)std::rand() / RAND_MAX * level->width;
    float py = (float)(level->depth + 10);
    float pz = (float)std::rand() / RAND_MAX * level->height;
    setPos(px, py, pz);
}

void Entity::setPos(float px, float py, float pz) {
    x = px; y = py; z = pz;
    float w = 0.3f;
    float h = 0.9f;
    bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
}

void Entity::turn(float xo_, float yo_) {
    yRot += xo_ * 0.15f;
    xRot -= yo_ * 0.15f;
    if (xRot < -90.0f) xRot = -90.0f;
    if (xRot > 90.0f) xRot = 90.0f;
}

void Entity::tick() {
    xo = x;
    yo = y;
    zo = z;
}

void Entity::move(float xa, float ya, float za) {
    float xaOrg = xa, yaOrg = ya, zaOrg = za;

    AABB expanded = bb.expand(xa, ya, za);
    auto aabbs = level->getCubes(expanded);

    for (auto& a : aabbs) ya = a.clipYCollide(bb, ya);
    bb.move(0, ya, 0);

    for (auto& a : aabbs) xa = a.clipXCollide(bb, xa);
    bb.move(xa, 0, 0);

    for (auto& a : aabbs) za = a.clipZCollide(bb, za);
    bb.move(0, 0, za);

    onGround = (yaOrg != ya) && (yaOrg < 0.0f);

    if (xaOrg != xa) xd = 0;
    if (yaOrg != ya) yd = 0;
    if (zaOrg != za) zd = 0;

    x = (bb.x0 + bb.x1) * 0.5f;
    y = bb.y0 + heightOffset;
    z = (bb.z0 + bb.z1) * 0.5f;
}

void Entity::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (dist < 0.01f) return;

    dist = speed / std::sqrt(dist);
    xa *= dist;
    za *= dist;

    float sin = std::sin(yRot * 3.141592653589793f / 180.0f);
    float cos = std::cos(yRot * 3.141592653589793f / 180.0f);

    xd += xa * cos - za * sin;
    zd += za * cos + xa * sin;
}