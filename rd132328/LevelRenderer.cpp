// LevelRenderer.cpp
#include "LevelRenderer.hpp"
#include "Frustum.hpp"
#include "Tile.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>

LevelRenderer::LevelRenderer(Level* level) : level(level) {
    level->addListener(this);

    xChunks = level->width / CHUNK_SIZE;
    yChunks = level->depth / CHUNK_SIZE;
    zChunks = level->height / CHUNK_SIZE;

    chunks.resize(xChunks * yChunks * zChunks);

    for (int x = 0; x < xChunks; x++) {
        for (int y = 0; y < yChunks; y++) {
            for (int z = 0; z < zChunks; z++) {
                int cx0 = x * CHUNK_SIZE;
                int cy0 = y * CHUNK_SIZE;
                int cz0 = z * CHUNK_SIZE;
                int cx1 = (x + 1) * CHUNK_SIZE;
                int cy1 = (y + 1) * CHUNK_SIZE;
                int cz1 = (z + 1) * CHUNK_SIZE;

                if (cx1 > level->width)  cx1 = level->width;
                if (cy1 > level->depth)  cy1 = level->depth;
                if (cz1 > level->height) cz1 = level->height;

                chunks[(x + y * xChunks) * zChunks + z] =
                    new Chunk(level, cx0, cy0, cz0, cx1, cy1, cz1);
            }
        }
    }
}

LevelRenderer::~LevelRenderer() {
    for (auto chunk : chunks) delete chunk;
    level->removeListener(this);
}

void LevelRenderer::init() {
    Chunk::initTexture();
    for (auto chunk : chunks) {
        chunk->lists = glGenLists(2);
    }
}

void LevelRenderer::render(Player* player, int layer) {
    Chunk::rebuiltThisFrame = 0;
    Frustum* frustum = Frustum::getFrustum();

    for (auto chunk : chunks) {
        if (frustum->cubeInFrustum(chunk->aabb)) {
            chunk->render(layer);
        }
    }
}

void LevelRenderer::pick(Player* player) {
    float r = 3.0f;
    AABB box = player->bb.grow(r, r, r);

    int px0 = (int)box.x0;
    int px1 = (int)(box.x1 + 1.0f);
    int py0 = (int)box.y0;
    int py1 = (int)(box.y1 + 1.0f);
    int pz0 = (int)box.z0;
    int pz1 = (int)(box.z1 + 1.0f);

    glInitNames();

    for (int x = px0; x < px1; x++) {
        glPushName(x);
        for (int y = py0; y < py1; y++) {
            glPushName(y);
            for (int z = pz0; z < pz1; z++) {
                glPushName(z);
                if (level->isSolidTile(x, y, z)) {
                    glPushName(0);
                    for (int i = 0; i < 6; i++) {
                        glPushName(i);
                        t.init();
                        Tile::rock.renderFace(t, x, y, z, i);
                        t.flush();
                        glPopName();
                    }
                    glPopName();
                }
                glPopName();
            }
            glPopName();
        }
        glPopName();
    }
}

void LevelRenderer::renderHit(HitResult* h) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    float alpha = (float)(sin(glfwGetTime() * 10.0) * 0.2 + 0.4);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    t.init();
    Tile::rock.renderFace(t, h->x, h->y, h->z, h->f);
    t.flush();

    glDisable(GL_BLEND);
}

void LevelRenderer::rebuildAll() {
    for (auto c : chunks) c->setDirty();
}

void LevelRenderer::setDirty(int x0, int y0, int z0, int x1, int y1, int z1) {
    x0 /= CHUNK_SIZE; x1 /= CHUNK_SIZE;
    y0 /= CHUNK_SIZE; y1 /= CHUNK_SIZE;
    z0 /= CHUNK_SIZE; z1 /= CHUNK_SIZE;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 >= xChunks) x1 = xChunks - 1;
    if (y1 >= yChunks) y1 = yChunks - 1;
    if (z1 >= zChunks) z1 = zChunks - 1;

    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
            for (int z = z0; z <= z1; z++) {
                chunks[(x + y * xChunks) * zChunks + z]->setDirty();
            }
        }
    }
}

void LevelRenderer::tileChanged(int x, int y, int z) {
    setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void LevelRenderer::lightColumnChanged(int x, int z, int y0, int y1) {
    setDirty(x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
}

void LevelRenderer::allChanged() {
    setDirty(0, 0, 0, level->width, level->depth, level->height);
}