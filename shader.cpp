#include "shader.hpp"
#include "matrix.hpp"
#include "vertex.hpp"
#include <cassert>
#include <iostream>

Shader::Shader(GLuint shaderID) : shaderID(shaderID)
{
    pos = glGetAttribLocation(shaderID, "position");
    assert(pos != 0xFFFFFFFF);
    normal = glGetAttribLocation(shaderID, "vertexNormal");
    assert(normal != 0xFFFFFFFF);
    wvp = glGetUniformLocation(shaderID, "WVP");
    assert(wvp != 0xFFFFFFFF);
    world = glGetUniformLocation(shaderID, "world");
    assert(world != 0xFFFFFFFF);

    lightColor = glGetUniformLocation(shaderID, "light.Color");
    assert(lightColor != 0xFFFFFFFF);
    ambientIntensity = glGetUniformLocation(shaderID, "light.AmbientIntensity");
    assert(ambientIntensity != 0xFFFFFFFF);
    lightDirection = glGetUniformLocation(shaderID, "light.Direction");
    assert(lightDirection != 0xFFFFFFFF);
    diffuseIntensity = glGetUniformLocation(shaderID, "light.DiffuseIntensity");
    assert(diffuseIntensity != 0xFFFFFFFF);

    //eyePos = glGetUniformLocation(shaderID, "eyePosition");
    //assert(eyePos != 0xFFFFFFFF);
    //specularIntensity = glGetUniformLocation(shaderID, "specularIntensity");
    //assert(specularIntensity != 0xFFFFFFFF);
    //specularPower = glGetUniformLocation(shaderID, "specularPower");
    //assert(specularPower != 0xFFFFFFFF);
}

Shader::~Shader()
{
    glDeleteProgram(shaderID);
}

void Shader::UploadMatrix(GLuint pos, const Matrix4f& mat) const
{
    glUniformMatrix4fv(pos, 1, GL_TRUE, &mat[0][0]);
}

void Shader::SetWVP(const Matrix4f& mat) const
{
    UploadMatrix(wvp, mat);
}

void Shader::SetWorld(const Matrix4f& mat) const
{
    UploadMatrix(world, mat);
}

/*void Shader::SetCameraPosition(const Vector3f& pos) const
{
    glUniform3fv(eyePos, 1, (const GLfloat*)&pos);
}*/

void Shader::SetVertexData()
{
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (const GLvoid*)20);
}

void Shader::Use(const DirectionalLight& light) const
{
    glUseProgram(shaderID);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(normal);
    glUniform3fv(lightColor, 1, &light.color.r);
    glUniform1f(ambientIntensity, light.ambientIntensity);
    glUniform3fv(lightDirection, 1, (const GLfloat*)&light.direction);
    glUniform1f(diffuseIntensity, light.diffuseIntensity);
}

void Shader::StopUsing() const
{
    glDisableVertexAttribArray(pos);
    glDisableVertexAttribArray(normal);
}
