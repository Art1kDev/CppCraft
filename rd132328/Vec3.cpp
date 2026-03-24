// Vec3.cpp
#include "Vec3.hpp"

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vec3 Vec3::interpolateTo(const Vec3& t, float p) const {
    float xt = x + (t.x - x) * p;
    float yt = y + (t.y - y) * p;
    float zt = z + (t.z - z) * p;
    return Vec3(xt, yt, zt);
}

void Vec3::set(float x_, float y_, float z_) {
    x = x_;
    y = y_;
    z = z_;
}