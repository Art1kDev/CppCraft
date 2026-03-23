// Chunk.cpp
#include "Chunk.hpp"
#include "Tile.hpp"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint Chunk::texture = 0;
Tesselator Chunk::t;
int Chunk::rebuiltThisFrame = 0;
int Chunk::updates = 0;

Chunk::Chunk(Level* level, int x0, int y0, int z0, int x1, int y1, int z1)
    : level(level), x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1),
      dirty(true), lists(0),
      aabb((float)x0, (float)y0, (float)z0, (float)x1, (float)y1, (float)z1) {
}

Chunk::~Chunk() {
    if (lists) glDeleteLists(lists, 2);
}

void Chunk::initTexture() {
    if (texture != 0) return;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int w, h, c;
    unsigned char* data = stbi_load("terrain.png", &w, &h, &c, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    } else {
        unsigned char pixels[256 * 256 * 4];
        for (int i = 0; i < 256 * 256 * 4; i++) pixels[i] = 255;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
}

void Chunk::rebuild(int layer) {
    if (rebuiltThisFrame == 2) return;

    dirty = false;
    updates++;
    rebuiltThisFrame++;

    glNewList(lists + layer, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    t.init();

    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
                if (level->isTile(x, y, z)) {
                    int tex = (y == level->depth * 2 / 3) ? 0 : 1;
                    if (tex == 0) {
                        Tile::rock.render(t, level, layer, x, y, z);
                    } else {
                        Tile::grass.render(t, level, layer, x, y, z);
                    }
                }
            }
        }
    }

    t.flush();
    glDisable(GL_TEXTURE_2D);
    glEndList();
}

void Chunk::render(int layer) {
    if (dirty) {
        rebuild(0);
        rebuild(1);
    }
    glCallList(lists + layer);
}

void Chunk::setDirty() {
    dirty = true;
}