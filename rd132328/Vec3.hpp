// Vec3.hpp
#pragma once

class Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 interpolateTo(const Vec3& t, float p) const;
    void set(float x, float y, float z);
};