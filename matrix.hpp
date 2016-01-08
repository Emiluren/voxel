#ifndef MATRIX_HPP
#define MATRIX_HPP

struct Matrix4fRow
{
    const float& operator[](int index) const { return v[index]; }
    float& operator[](int index) { return v[index]; }
    float v[4];
};

class Matrix4f
{
public:
    const Matrix4fRow& operator[](int index) const { return rows[index]; }
    Matrix4fRow& operator[](int index) { return rows[index]; }
    Matrix4fRow rows[4];

    Matrix4f operator*(const Matrix4f& Right) const;
};

Matrix4f scale(float x, float y, float z);
Matrix4f translate(float x, float y, float z);

Matrix4f rotateX(float radians);
Matrix4f rotateY(float radians);
Matrix4f rotateZ(float radians);

Matrix4f persProj(float aspectRatio, float nearClip, float farClip, float FOV);

#endif // MATRIX_HPP
