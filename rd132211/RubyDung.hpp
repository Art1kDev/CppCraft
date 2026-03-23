// RubyDung.hpp
#ifndef RUBYDUNG_HPP
#define RUBYDUNG_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <cmath>
#include "Timer.hpp"
#include "Level.hpp"
#include "LevelRenderer.hpp"
#include "Player.hpp"
#include "HitResult.hpp"

class RubyDung {
private:
    GLFWwindow* window;
    int width;
    int height;
    Timer timer;
    Level level;
    LevelRenderer levelRenderer;
    Player player;
    HitResult* hitResult;
    float fogColor[4];
    bool leftPressed;
    bool rightPressed;
    int streamRadius;

    void init();
    void destroy();
    void moveCameraToPlayer(float a);
    void setupCamera(float a);
    void setupPickCamera(float a, int x, int y);
    void pick(float a);
    void render(float a);
    void tick();

public:
    RubyDung();
    ~RubyDung();
    void run();
};

#endif