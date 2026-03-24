//Timer.hpp
#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
public:
    float ticksPerSecond;
    float timeScale;
    float fps;
    float passedTime;
    int ticks;
    float a;
    long long lastTime;

    Timer(float ticksPerSecond);
    void advanceTime();
};

#endif