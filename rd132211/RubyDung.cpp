// RubyDung.cpp
#include "RubyDung.hpp"
#include <iostream>

RubyDung::RubyDung() : level(256, 256, 64), levelRenderer(&level), player(&level), hitResult(nullptr), timer(60.0f), leftPressed(false), rightPressed(false), streamRadius(2) {
    fogColor[0] = 0.5f;
    fogColor[1] = 0.8f;
    fogColor[2] = 1.0f;
    fogColor[3] = 1.0f;
    width = 1024;
    height = 768;
}

RubyDung::~RubyDung() {
    destroy();
}
void RubyDung::init() {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    window = glfwCreateWindow(width, height, "RubyDung", nullptr, nullptr);
    if (!window) { glfwTerminate(); throw std::runtime_error("Failed to create window"); }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) throw std::runtime_error("Failed to initialize GLEW");

    glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    levelRenderer.init();

    player.setPos((float)level.width / 2.0f, 60.0f, (float)level.height / 2.0f);
    player.xRot = 20.0f;
}

void RubyDung::destroy() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void RubyDung::moveCameraToPlayer(float a) {
    glTranslatef(0.0f, 0.0f, -0.3f);
    glRotatef(player.xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(player.yRot, 0.0f, 1.0f, 0.0f);

    float x = player.xo + (player.x - player.xo) * a;
    float y = player.yo + (player.y - player.yo) * a;
    float z = player.zo + (player.z - player.zo) * a;
    glTranslatef(-x, -y, -z);
}

void RubyDung::setupCamera(float a) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov = 70.0f;
    float aspect = (float)width / (float)height;
    float nearPlane = 0.05f;
    float farPlane = 1000.0f;
    float f = 1.0f / tanf(fov * 3.14159f / 360.0f);

    float perspective[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farPlane + nearPlane) / (nearPlane - farPlane), -1,
        0, 0, (2 * farPlane * nearPlane) / (nearPlane - farPlane), 0
    };

    glLoadMatrixf(perspective);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(a);
}

void RubyDung::setupPickCamera(float a, int x, int y) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);

    float fov = 70.0f;
    float aspect = (float)width / (float)height;
    float nearPlane = 0.05f;
    float farPlane = 1000.0f;
    float f = 1.0f / tanf(fov * 3.14159f / 360.0f);

    float perspective[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farPlane + nearPlane) / (nearPlane - farPlane), -1,
        0, 0, (2 * farPlane * nearPlane) / (nearPlane - farPlane), 0
    };

    glMultMatrixf(perspective);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(a);
}

void RubyDung::pick(float a) {
    GLuint selectBuffer[2000];
    glSelectBuffer(2000, selectBuffer);
    glRenderMode(GL_SELECT);
    setupPickCamera(a, width / 2, height / 2);
    levelRenderer.pick(&player);

    int hits = glRenderMode(GL_RENDER);
    GLuint* ptr = selectBuffer;

    GLuint closest = 0xFFFFFFFF;
    HitResult tempResult;
    bool hitFound = false;

    for (int i = 0; i < hits; i++) {
        int nameCount = *ptr++;
        GLuint minZ = *ptr++;
        ptr++;

        if (minZ < closest) {
            closest = minZ;
            if (nameCount >= 5) {
                tempResult.x = *ptr++;
                tempResult.y = *ptr++;
                tempResult.z = *ptr++;
                tempResult.o = *ptr++;
                tempResult.f = *ptr++;
                hitFound = true;
            } else {
                ptr += nameCount;
            }
        } else {
            ptr += nameCount;
        }
    }

    if (hitFound) {
        if (!hitResult) hitResult = new HitResult(tempResult);
        else *hitResult = tempResult;
    } else {
        if (hitResult) {
            delete hitResult;
            hitResult = nullptr;
        }
    }
}

void RubyDung::render(float a) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    width = fbWidth;
    height = fbHeight;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    static double lastX = xpos, lastY = ypos;
    float xo = (float)(xpos - lastX);
    float yo = (float)(ypos - lastY);
    lastX = xpos;
    lastY = ypos;

    player.turn(xo, yo);

    setupCamera(a);
    pick(a);

    bool right = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (right && !rightPressed && hitResult) {
        level.setTile(hitResult->x, hitResult->y, hitResult->z, 0);
    }
    rightPressed = right;

    bool left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (left && !leftPressed && hitResult) {
        int x = hitResult->x, y = hitResult->y, z = hitResult->z;
        switch (hitResult->f) {
            case 0: y--; break; case 1: y++; break;
            case 2: z--; break; case 3: z++; break;
            case 4: x--; break; case 5: x++; break;
        }
        level.setTile(x, y, z, 1);
    }
    leftPressed = left;

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) level.save();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    setupCamera(a);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 20.0f);
    glFogf(GL_FOG_END, 200.0f);
    glFogfv(GL_FOG_COLOR, fogColor);

    levelRenderer.render(&player, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    levelRenderer.render(&player, 1);
    glDisable(GL_BLEND);

    glDisable(GL_FOG);

    if (hitResult) levelRenderer.renderHit(hitResult);

    glfwSwapBuffers(window);
}

void RubyDung::tick() {
    player.tick();
}

void RubyDung::run() {
    init();

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        timer.advanceTime();

        for (int i = 0; i < timer.ticks; i++) {
            tick();
        }

        render(timer.a);
        glfwPollEvents();
    }

    destroy();
}