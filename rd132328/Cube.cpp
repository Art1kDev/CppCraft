// Cube.cpp
#include "Cube.hpp"
#include <GL/glew.h>

Cube::Cube(int xTexOffs_, int yTexOffs_)
    : xTexOffs(xTexOffs_), yTexOffs(yTexOffs_),
      x(0), y(0), z(0),
      xRot(0), yRot(0), zRot(0) {}

void Cube::setTexOffs(int xTexOffs_, int yTexOffs_) {
    xTexOffs = xTexOffs_;
    yTexOffs = yTexOffs_;
}

void Cube::addBox(float x0, float y0, float z0, int w, int h, int d) {
    vertices.clear();
    polygons.clear();

    float x1 = x0 + (float)w;
    float y1 = y0 + (float)h;
    float z1 = z0 + (float)d;

    Vertex u0(x0, y0, z0, 0.0f, 0.0f);
    Vertex u1(x1, y0, z0, 0.0f, 8.0f);
    Vertex u2(x1, y1, z0, 8.0f, 8.0f);
    Vertex u3(x0, y1, z0, 8.0f, 0.0f);

    Vertex l0(x0, y0, z1, 0.0f, 0.0f);
    Vertex l1(x1, y0, z1, 0.0f, 8.0f);
    Vertex l2(x1, y1, z1, 8.0f, 8.0f);
    Vertex l3(x0, y1, z1, 8.0f, 0.0f);

    vertices = {u0,u1,u2,u3,l0,l1,l2,l3};

    std::array<Vertex,4> p0 = { l1, u1, u2, l2 };
    std::array<Vertex,4> p1 = { u0, l0, l3, u3 };
    std::array<Vertex,4> p2 = { l1, l0, u0, u1 };
    std::array<Vertex,4> p3 = { u2, u3, l3, l2 };
    std::array<Vertex,4> p4 = { u1, u0, u3, u2 };
    std::array<Vertex,4> p5 = { l0, l1, l2, l3 };

    polygons.emplace_back(p0, xTexOffs + d + w,     yTexOffs + d,       xTexOffs + d + w + d,     yTexOffs + d + h);
    polygons.emplace_back(p1, xTexOffs + 0,         yTexOffs + d,       xTexOffs + d,             yTexOffs + d + h);
    polygons.emplace_back(p2, xTexOffs + d,         yTexOffs + 0,       xTexOffs + d + w,         yTexOffs + d);
    polygons.emplace_back(p3, xTexOffs + d + w,     yTexOffs + 0,       xTexOffs + d + w + w,     yTexOffs + d);
    polygons.emplace_back(p4, xTexOffs + d,         yTexOffs + d,       xTexOffs + d + w,         yTexOffs + d + h);
    polygons.emplace_back(p5, xTexOffs + d + w + d, yTexOffs + d,       xTexOffs + d + w + d + w, yTexOffs + d + h);
}

void Cube::setPos(float x_, float y_, float z_) {
    x = x_;
    y = y_;
    z = z_;
}

void Cube::render() {
    float c = 57.29578f;
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(zRot * c, 0.0f, 0.0f, 1.0f);
    glRotatef(yRot * c, 0.0f, 1.0f, 0.0f);
    glRotatef(xRot * c, 1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    for (auto& p : polygons) {
        p.render();
    }
    glEnd();

    glPopMatrix();
}