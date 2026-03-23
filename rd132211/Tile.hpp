//Tile.hpp
#ifndef TILE_HPP
#define TILE_HPP

#include "Tesselator.hpp"
#include "Level.hpp"

class Tile {
public:
    static Tile rock;
    static Tile grass;
    int tex;

    Tile(int tex);
    void render(Tesselator& t, Level* level, int layer, int x, int y, int z);
    void renderFace(Tesselator& t, int x, int y, int z, int face);
};

#endif