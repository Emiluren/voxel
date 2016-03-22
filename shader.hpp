#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "lib.h"
#include "vector.hpp"

class Matrix4f;

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

struct DirectionalLight {
    Color color;
    float ambientIntensity;
    Vector3f direction;
    float diffuseIntensity;
};

class Shader
{
public:
    Shader(GLuint shaderID);
    virtual ~Shader();
    void UploadMatrix(GLuint pos, const Matrix4f& mat) const;
    void SetWVP(const Matrix4f& mat) const;
    void SetWorld(const Matrix4f& mat) const;
    //void SetCameraPosition(const Vector3f& pos) const;
    virtual void Use(const DirectionalLight& light) const;
    virtual void SetVertexData();
    virtual void StopUsing() const;

    GLuint shaderID;

protected:
    GLuint pos;
    GLuint wvp;
    GLuint world;
    GLuint normal;

    GLuint lightColor;
    GLuint ambientIntensity;
    GLuint lightDirection;
    GLuint diffuseIntensity;

    //GLuint eyePos;
    //GLuint specularIntensity;
    //GLuint specularPower;
};

#endif // SHADERS_HPP
