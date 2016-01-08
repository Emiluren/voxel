#include "vector.hpp"
#include "quaternion.hpp"
#include "macros.h"
#include <cmath>

Vector2f& Vector2f::operator+=(const Vector2f& right)
{
    x += right.x;
    y += right.y;

    return *this;
}

Vector2f Vector2f::operator*(const float right) const
{
    return Vector2f(x * right, y * right);
}

Vector2f& Vector2f::operator*=(const float right)
{
    *this = *this * right;
    return *this;
}

/*Vector3f& Vector3f::operator=(const Vector3f& right)
{
    x = right.x;
    y = right.y;
    z = right.z;

    return *this;
}*/

Vector3f& Vector3f::operator+=(const Vector3f& right)
{
    x += right.x;
    y += right.y;
    z += right.z;

    return *this;
}

Vector3f Vector3f::operator*(const float right) const
{
    return Vector3f(x * right, y * right, z * right);
}

Vector3f& Vector3f::operator*=(const float right)
{
    *this = *this * right;
    return *this;
}

Vector3f& Vector3f::operator-=(const Vector3f& right)
{
    x -= right.x;
    y -= right.y;
    z -= right.z;

    return *this;
}

float Vector3f::Length() const
{
    return sqrtf(x*x + y*y + z*z);
}

float Vector3f::LengthSqrd() const
{
    return x*x + y*y + z*z;
}

Vector3f Vector3f::Cross(const Vector3f& v) const
{
    Vector3f ret;
    ret.x = y * v.z - z * v.y;
    ret.y = z * v.x - x * v.z;
    ret.z = x * v.y - y * v.x;
    return ret;
}

Vector3f& Vector3f::Normalize()
{
    float length = sqrtf(x*x + y*y + z*z);

    x /= length;
    y /= length;
    z /= length;

    return *this;
}

Vector3f& Vector3f::Rotate(float angle, const Vector3f& axis)
{
    const float SinHalfAngle = sinf(ToRadian(angle/2));
    const float CosHalfAngle = cosf(ToRadian(angle/2));

    const float Rx = axis.x * SinHalfAngle;
    const float Ry = axis.y * SinHalfAngle;
    const float Rz = axis.z * SinHalfAngle;
    const float Rw = CosHalfAngle;
    Quaternion rotQ(Rx, Ry, Rz, Rw);

    Quaternion conjQ = rotQ.Conjugate();
    Quaternion W = rotQ * (*this) * conjQ;

    x = W.x;
    y = W.y;
    z = W.z;

    return *this;
}
