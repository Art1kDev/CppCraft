//LevelListener.hpp
#ifndef LEVELLISTENER_HPP
#define LEVELLISTENER_HPP

class LevelListener {
public:
    virtual ~LevelListener() {}
    virtual void tileChanged(int x, int y, int z) = 0;
    virtual void lightColumnChanged(int x, int z, int y0, int y1) = 0;
    virtual void allChanged() = 0;
};

#endif