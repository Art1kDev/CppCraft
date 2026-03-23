//Frustum.hpp
#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include "AABB.hpp"

class Frustum {
private:
    float m_Frustum[6][4];
    static Frustum instance;
    
    Frustum();
    void normalizePlane(int side);
    void calculateFrustum();
    
public:
    static Frustum* getFrustum();
    bool pointInFrustum(float x, float y, float z);
    bool sphereInFrustum(float x, float y, float z, float radius);
    bool cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);
    bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);
    bool cubeInFrustum(const AABB& aabb);
};

#endif