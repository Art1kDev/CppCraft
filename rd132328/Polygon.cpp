// Polygon.cpp
#include "Polygon.hpp"
#include <GL/glew.h>

Polygon::Polygon(const std::array<Vertex,4>& verts)
    : vertices(verts), vertexCount(4) {}

Polygon::Polygon(const std::array<Vertex,4>& verts, int u0, int v0, int u1, int v1)
    : vertices(verts), vertexCount(4) {
    vertices[0] = vertices[0].remap((float)u1, (float)v0);
    vertices[1] = vertices[1].remap((float)u0, (float)v0);
    vertices[2] = vertices[2].remap((float)u0, (float)v1);
    vertices[3] = vertices[3].remap((float)u1, (float)v1);
}

void Polygon::render() {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 3; i >= 0; --i) {
        const Vertex& v = vertices[i];
        glTexCoord2f(v.u / 64.0f, v.v / 32.0f);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
}