// Vertex.hpp
#pragma once
#include "Vec3.hpp"

class Vertex {
public:
    Vec3 pos;
    float u;
    float v;

    Vertex(float x, float y, float z, float u, float v);
    Vertex(const Vec3& pos, float u, float v);
    Vertex(const Vertex& vertex, float u, float v);

    Vertex remap(float u, float v) const;
};