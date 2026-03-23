//Timer.cpp
#include "Timer.hpp"
#include <chrono>

Timer::Timer(float ticksPerSecond)
    : ticksPerSecond(ticksPerSecond), timeScale(1.0f), fps(0.0f), passedTime(0.0f), ticks(0), a(0.0f) {
    lastTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

void Timer::advanceTime() {
    long long now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    long long passedNs = now - lastTime;
    lastTime = now;

    if (passedNs < 0) passedNs = 0;
    if (passedNs > 1000000000LL) passedNs = 1000000000LL;

    fps = 1000000000.0f / passedNs;
    passedTime += (float)passedNs * timeScale * ticksPerSecond / 1000000000.0f;
    ticks = (int)passedTime;

    if (ticks > 100) ticks = 100;

    passedTime -= (float)ticks;
    a = passedTime;
}