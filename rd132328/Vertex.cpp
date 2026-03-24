// Vertex.cpp
#include "Vertex.hpp"

Vertex::Vertex(float x, float y, float z, float u_, float v_)
    : pos(x,y,z), u(u_), v(v_) {}

Vertex::Vertex(const Vec3& pos_, float u_, float v_)
    : pos(pos_), u(u_), v(v_) {}

Vertex::Vertex(const Vertex& vertex, float u_, float v_)
    : pos(vertex.pos), u(u_), v(v_) {}

Vertex Vertex::remap(float u_, float v_) const {
    return Vertex(*this, u_, v_);
}