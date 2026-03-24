// Tesselator.cpp
#include "Tesselator.hpp"

Tesselator::Tesselator() {
    vertices.reserve(65536);
    init();
}

void Tesselator::init() {
    vertices.clear();
    u = 0; v = 0;
    r = 1; g = 1; b = 1;
    hasTex = false;
    hasCol = false;
}

void Tesselator::tex(float u, float v) {
    this->u = u;
    this->v = v;
    hasTex = true;
}

void Tesselator::color(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
    hasCol = true;
}

void Tesselator::vertex(float x, float y, float z) {
    Vertex vert;
    vert.x = x; vert.y = y; vert.z = z;
    vert.u = u; vert.v = v;
    vert.r = r; vert.g = g; vert.b = b;
    vert.hasTex = hasTex;
    vert.hasCol = hasCol;
    vertices.push_back(vert);
}

void Tesselator::flush() {
    if (vertices.empty()) return;

    glBegin(GL_QUADS);
    for (auto& vert : vertices) {
        if (vert.hasCol) glColor3f(vert.r, vert.g, vert.b);
        if (vert.hasTex) glTexCoord2f(vert.u, vert.v);
        glVertex3f(vert.x, vert.y, vert.z);
    }
    glEnd();

    init();
}