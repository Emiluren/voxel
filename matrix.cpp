#include "matrix.hpp"
#include "macros.h"
#include <cmath>

Matrix4f Matrix4f::operator*(const Matrix4f& Right) const
{
    Matrix4f ret;
    for (unsigned int i = 0 ; i < 4 ; i++)
    {
        for (unsigned int j = 0 ; j < 4 ; j++)
        {
            ret[i][j] = rows[i][0] * Right[0][j] +
                        rows[i][1] * Right[1][j] +
                        rows[i][2] * Right[2][j] +
                        rows[i][3] * Right[3][j];
       }
    }

    return ret;
}

Matrix4f translate(float x, float y, float z)
{
    Matrix4f ret;
    ret[0][0] = 1.0f; ret[0][1] = 0.0f; ret[0][2] = 0.0f; ret[0][3] = x;
    ret[1][0] = 0.0f; ret[1][1] = 1.0f; ret[1][2] = 0.0f; ret[1][3] = y;
    ret[2][0] = 0.0f; ret[2][1] = 0.0f; ret[2][2] = 1.0f; ret[2][3] = z;
    ret[3][0] = 0.0f; ret[3][1] = 0.0f; ret[3][2] = 0.0f; ret[3][3] = 1.0f;
    return ret;
}

Matrix4f scale(float x, float y, float z)
{
    Matrix4f ret;
    ret[0][0] = x;    ret[0][1] = 0.0f; ret[0][2] = 0.0f; ret[0][3] = 0.0f;
    ret[1][0] = 0.0f; ret[1][1] = y;    ret[1][2] = 0.0f; ret[1][3] = 0.0f;
    ret[2][0] = 0.0f; ret[2][1] = 0.0f; ret[2][2] = z;    ret[2][3] = 0.0f;
    ret[3][0] = 0.0f; ret[3][1] = 0.0f; ret[3][2] = 0.0f; ret[3][3] = 1.0f;
    return ret;
}

Matrix4f rotateY(float rad)
{
    Matrix4f ret;
    ret[0][0] = cosf(rad); ret[0][1] = 0.0f; ret[0][2] = -sinf(rad); ret[0][3] = 0.0f;
    ret[1][0] = 0.0f;      ret[1][1] = 1.0f; ret[1][2] = 0.0f;       ret[1][3] = 0.0f;
    ret[2][0] = sinf(rad); ret[2][1] = 0.0f; ret[2][2] = cosf(rad);  ret[2][3] = 0.0f;
    ret[3][0] = 0.0f;      ret[3][1] = 0.0f; ret[3][2] = 0.0f;       ret[3][3] = 1.0f;
    return ret;
}

Matrix4f persProj(float aspectRatio, float nearClip, float farClip, float FOV)
{
    Matrix4f ret;
    const float range = nearClip - farClip;
    const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

    ret[0][0] = 1.0f / (tanHalfFOV * aspectRatio);
    ret[0][1] = 0.0f;
    ret[0][2] = 0.0f;
    ret[0][3] = 0.0f;

    ret[1][0] = 0.0f;
    ret[1][1] = 1.0f / tanHalfFOV;
    ret[1][2] = 0.0f;
    ret[1][3] = 0.0f;

    ret[2][0] = 0.0f;
    ret[2][1] = 0.0f;
    ret[2][2] = (-nearClip - farClip) / range;
    ret[2][3] = 2.0f * farClip * nearClip / range;

    ret[3][0] = 0.0f;
    ret[3][1] = 0.0f;
    ret[3][2] = 1.0f;
    ret[3][3] = 0.0f;
    
    return ret;
}
