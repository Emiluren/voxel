#include "texturedShader.hpp"
#include "vertex.hpp"
#include <cassert>

TexturedShader::TexturedShader(GLuint shaderID) : Shader(shaderID)
{
    uv = glGetAttribLocation(shaderID, "vertexUV");
    assert(uv != 0xFFFFFFFF);
    textureID = glGetUniformLocation(shaderID, "textureSampler");
    assert(textureID != 0xFFFFFFFF);
}

void TexturedShader::Use(const DirectionalLight& light) const
{
    Shader::Use(light);
    glEnableVertexAttribArray(uv);
    //glUniform1f(specularIntensity, 1.0f);
    //glUniform1f(specularPower, 32);
}

void TexturedShader::SetVertexData()
{
    glUniform1i(textureID, 0);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
}

void TexturedShader::StopUsing() const
{
    Shader::StopUsing();
    glDisableVertexAttribArray(uv);
}
