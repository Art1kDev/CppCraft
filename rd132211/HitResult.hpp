//HitResult.hpp
#ifndef HITRESULT_HPP
#define HITRESULT_HPP

struct HitResult {
    int x, y, z, o, f;
    
    HitResult() : x(0), y(0), z(0), o(0), f(0) {}
    HitResult(int x, int y, int z, int o, int f) : x(x), y(y), z(z), o(o), f(f) {}
};

#endif