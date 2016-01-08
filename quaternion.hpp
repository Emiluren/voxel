#ifndef QUATERNION_HPP
#define QUATERNION_HPP

class Vector3f;

class Quaternion
{
public:
    Quaternion(float x, float y, float z, float w);
    float x, y, z, w;
    
    void Normalize();
    Quaternion Conjugate();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);
Quaternion operator*(const Quaternion& q, const Vector3f& v);

#endif // QUATERNION_HPP
