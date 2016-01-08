#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector2f
{
public:
    Vector2f() : x(0), y(0) {}
    Vector2f(float x, float y) { this->x = x; this->y = y; }
    float x, y;

    Vector2f& operator+=(const Vector2f& right);
    Vector2f operator*(const float right) const;
    Vector2f& operator*=(const float right);
    // Vector2f& operator-=(const Vector2f& right);
    // Vector2f operator-(const Vector2f& right);
};

class Vector3f
{
public:
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
    float x,y,z;

    //Vector3f& operator=(const Vector3f& right);
    Vector3f& operator+=(const Vector3f& right);
    Vector3f operator*(const float right) const;
    Vector3f& operator*=(const float right);
    Vector3f& operator-=(const Vector3f& right);

    Vector3f Cross(const Vector3f& v) const;
    float Length() const;
    float LengthSqrd() const;
    Vector3f& Normalize();
    Vector3f& Rotate(float angle, const Vector3f& axis);
};

inline Vector3f operator-(Vector3f left, const Vector3f& right)
{
    left -= right;
    return left;
}

inline Vector3f operator+(Vector3f left, const Vector3f& right)
{
    left += right;
    return left;
}

#endif // VECTOR_HPP
