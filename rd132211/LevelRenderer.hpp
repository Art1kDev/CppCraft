//LevelRenderer.hpp
#ifndef LEVELRENDERER_HPP
#define LEVELRENDERER_HPP

#include "Level.hpp"
#include "LevelListener.hpp"
#include "Chunk.hpp"
#include "Player.hpp"
#include "HitResult.hpp"
#include "Tesselator.hpp"
#include <vector>

class LevelRenderer : public LevelListener {
private:
    static const int CHUNK_SIZE = 16;
    Level* level;
    int xChunks, yChunks, zChunks;
    Tesselator t;

    void setDirty(int x0, int y0, int z0, int x1, int y1, int z1);

public:
    std::vector<Chunk*> chunks;

    LevelRenderer(Level* level);
    ~LevelRenderer();

    void init();
    void render(Player* player, int layer);
    void pick(Player* player);
    void renderHit(HitResult* h);
    void rebuildAll();

    void tileChanged(int x, int y, int z) override;
    void lightColumnChanged(int x, int z, int y0, int y1) override;
    void allChanged() override;
};

#endif