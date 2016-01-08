#include "quaternion.hpp"
#include "vector.hpp"
#include <cmath>

Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::Normalize()
{
    float length = sqrtf(x * x + y * y + z * z + w * w);

    x /= length;
    y /= length;
    z /= length;
    w /= length;
}

Quaternion Quaternion::Conjugate()
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
    const float x = l.x*r.w + l.w*r.x + l.y*r.z - l.z*r.y;
    const float y = l.y*r.w + l.w*r.y + l.z*r.x - l.x*r.z;
    const float z = l.z*r.w + l.w*r.z + l.x*r.y - l.y*r.x;
    const float w = l.w*r.w - l.x*r.x - l.y*r.y - l.z*r.z;

    return Quaternion(x, y, z, w);
}

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
    const float w = - q.x*v.x - q.y*v.y - q.z*v.z;
    const float x =   q.w*v.x + q.y*v.z - q.z*v.y;
    const float y =   q.w*v.y + q.z*v.x - q.x*v.z;
    const float z =   q.w*v.z + q.x*v.y - q.y*v.x;

    return Quaternion(x, y, z, w);
}
