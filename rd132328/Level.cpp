// Level.cpp
#include "Level.hpp"
#include "LevelListener.hpp"
#include <fstream>
#include <algorithm>

Level::Level(int w, int h, int d) : width(w), height(h), depth(d) {
    blocks.resize(w * h * d);
    lightDepths.resize(w * h);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
                int i = (y * height + z) * width + x;
                blocks[i] = (y <= d * 2 / 3) ? 1 : 0;
            }
        }
    }

    calcLightDepths(0, 0, w, h);

    if (!load()) {
        save();
    }
}

bool Level::load() {
    std::ifstream file("level.dat", std::ios::binary);
    if (!file) return false;

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size != (std::streamsize)(width * height * depth)) return false;

    file.read(blocks.data(), blocks.size());
    file.close();
    calcLightDepths(0, 0, width, height);
    return true;
}

void Level::save() {
    std::ofstream file("level.dat", std::ios::binary | std::ios::trunc);
    if (file) {
        file.write(blocks.data(), blocks.size());
        file.close();
    }
}

void Level::calcLightDepths(int x0, int y0, int x1, int y1) {
    for (int x = x0; x < x0 + x1; x++) {
        for (int z = y0; z < y0 + y1; z++) {
            if (x >= 0 && x < width && z >= 0 && z < height) {
                int oldDepth = lightDepths[x + z * width];
                int y;
                for (y = depth - 1; y > 0 && !isLightBlocker(x, y, z); y--);
                lightDepths[x + z * width] = y;

                if (oldDepth != y) {
                    int yl0 = std::min(oldDepth, y);
                    int yl1 = std::max(oldDepth, y);
                    for (auto listener : levelListeners) {
                        listener->lightColumnChanged(x, z, yl0, yl1);
                    }
                }
            }
        }
    }
}

void Level::addListener(LevelListener* listener) {
    levelListeners.push_back(listener);
}

void Level::removeListener(LevelListener* listener) {
    levelListeners.erase(std::remove(levelListeners.begin(), levelListeners.end(), listener), levelListeners.end());
}

bool Level::isTile(int x, int y, int z) const {
    if (x >= 0 && y >= 0 && z >= 0 && x < width && y < depth && z < height) {
        return blocks[(y * height + z) * width + x] == 1;
    }
    return false;
}

bool Level::isSolidTile(int x, int y, int z) const {
    return isTile(x, y, z);
}

bool Level::isLightBlocker(int x, int y, int z) const {
    return isSolidTile(x, y, z);
}

std::vector<AABB> Level::getCubes(const AABB& aabb) const {
    std::vector<AABB> result;
    int x0 = (int)aabb.x0;
    int x1 = (int)(aabb.x1 + 1.0f);
    int y0 = (int)aabb.y0;
    int y1 = (int)(aabb.y1 + 1.0f);
    int z0 = (int)aabb.z0;
    int z1 = (int)(aabb.z1 + 1.0f);

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 > width) x1 = width;
    if (y1 > depth) y1 = depth;
    if (z1 > height) z1 = height;

    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
                if (isSolidTile(x, y, z)) {
                    result.push_back(AABB((float)x, (float)y, (float)z, (float)(x+1), (float)(y+1), (float)(z+1)));
                }
            }
        }
    }
    return result;
}

float Level::getBrightness(int x, int y, int z) const {
    if (x >= 0 && y >= 0 && z >= 0 && x < width && y < depth && z < height) {
        return y < lightDepths[x + z * width] ? 0.8f : 1.0f;
    }
    return 1.0f;
}

void Level::setTile(int x, int y, int z, int type) {
    if (x < 0 || y < 0 || z < 0 || x >= width || y >= depth || z >= height) return;
    blocks[(y * height + z) * width + x] = (char)type;
    calcLightDepths(x, z, 1, 1);
    for (auto listener : levelListeners) {
        listener->tileChanged(x, y, z);
    }
}