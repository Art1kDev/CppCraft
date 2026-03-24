#include "Textures.hpp"
#include <GL/glew.h>
#include <cstdio>

#include "stb_image.h"  

std::unordered_map<std::string, unsigned int> Textures::idMap;

unsigned int Textures::loadTexture(const char* path) {
    return loadTexture(path, GL_NEAREST);
}

unsigned int Textures::loadTexture(const char* path, int mode) {
    if (idMap.find(path) != idMap.end()) {
        return idMap[path];
    }

    int w, h, channels;
    unsigned char* image = stbi_load(path, &w, &h, &channels, 4);

    if (!image) {
        printf("Failed to load texture: %s\n", path);
        return 0;
    }

    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    if (mode == GL_NEAREST_MIPMAP_NEAREST || mode == GL_LINEAR_MIPMAP_LINEAR) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(image);

    idMap[path] = id;
    printf("%s -> %u\n", path, id);

    return id;
}