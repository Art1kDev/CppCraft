//Tesselator.hpp
#ifndef TESSELATOR_HPP
#define TESSELATOR_HPP

#include <GL/glew.h>
#include <vector>

class Tesselator {
private:
    struct Vertex {
        float x, y, z;
        float u, v;
        float r, g, b;
        bool hasTex;
        bool hasCol;
    };

    std::vector<Vertex> vertices;
    float u, v;
    float r, g, b;
    bool hasTex;
    bool hasCol;

public:
    Tesselator();
    void init();
    void tex(float u, float v);
    void color(float r, float g, float b);
    void vertex(float x, float y, float z);
    void flush();
};

#endif