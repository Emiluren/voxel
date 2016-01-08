#include "mesh.hpp"
#include "vertex.hpp"
#include "shader.hpp"

void Mesh::Render(Shader* shader)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    shader->SetVertexData();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
}
