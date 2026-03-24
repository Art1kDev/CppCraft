#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <string>
#include <unordered_map>

class Textures {
private:
    static std::unordered_map<std::string, unsigned int> idMap;

public:
    static unsigned int loadTexture(const char* path);
    static unsigned int loadTexture(const char* path, int mode);
};

#endif