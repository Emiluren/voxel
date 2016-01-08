#ifndef TEXTUREDSHADER
#define TEXTUREDSHADER

#include "shader.hpp"

class TexturedShader : public Shader
{
public:
    TexturedShader(GLuint shaderID);
    void Use(const DirectionalLight& light) const;
    void SetVertexData();
    void StopUsing() const;

private:
    GLuint uv;
    GLuint textureID;
};

#endif // TEXTUREDSHADER
