//Chunk.hpp
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Level.hpp"
#include "AABB.hpp"
#include "Tesselator.hpp"
#include <GL/glew.h>

class Chunk {
public:
    static GLuint texture;
    static Tesselator t;
    static int rebuiltThisFrame;
    static int updates;

    Level* level;
    int x0, y0, z0;
    int x1, y1, z1;
    bool dirty;
    GLuint lists;
    AABB aabb;

    Chunk(Level* level, int x0, int y0, int z0, int x1, int y1, int z1);
    ~Chunk();

    static void initTexture();
    void rebuild(int layer);
    void render(int layer);
    void setDirty();
};

#endif