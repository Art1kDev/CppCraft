//Level.hpp
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>
#include "AABB.hpp"

class LevelListener;

class Level {
private:
    std::vector<char> blocks;
    std::vector<int> lightDepths;
    std::vector<LevelListener*> levelListeners;

public:
    const int width, height, depth;

    Level(int w, int h, int d);
    bool load();
    void save();
    void calcLightDepths(int x0, int y0, int x1, int y1);
    void addListener(LevelListener* listener);
    void removeListener(LevelListener* listener);
    bool isTile(int x, int y, int z) const;
    bool isSolidTile(int x, int y, int z) const;
    bool isLightBlocker(int x, int y, int z) const;
    std::vector<AABB> getCubes(const AABB& aabb) const;
    float getBrightness(int x, int y, int z) const;
    void setTile(int x, int y, int z, int type);
};

#endif