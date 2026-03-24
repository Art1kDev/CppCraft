// Polygon.hpp
#pragma once
#include "Vertex.hpp"
#include <array>

class Polygon {
public:
    std::array<Vertex,4> vertices;
    int vertexCount;

    Polygon(const std::array<Vertex,4>& vertices);
    Polygon(const std::array<Vertex,4>& vertices, int u0, int v0, int u1, int v1);

    void render();
};