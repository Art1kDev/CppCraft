// Cube.hpp
#pragma once
#include <vector>
#include "Vertex.hpp"
#include "Polygon.hpp"

class Cube {
public:
    std::vector<Vertex> vertices;
    std::vector<Polygon> polygons;
    int xTexOffs;
    int yTexOffs;
    float x;
    float y;
    float z;
    float xRot;
    float yRot;
    float zRot;

    Cube(int xTexOffs, int yTexOffs);

    void setTexOffs(int xTexOffs, int yTexOffs);
    void addBox(float x0, float y0, float z0, int w, int h, int d);
    void setPos(float x, float y, float z);
    void render();
};