#include "block.hpp"
#include "vertex.hpp"
#include "texturedShader.hpp"

/*Block::Block(BlockType type) : type(type)
{
    Vertex vertices[] = {
        Vertex(Vector3f( 1.0f,  0.0f,  1.0f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f( 0.0f,  0.0f,  1.0f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f( 0.0f,  1.0f,  1.0f), Vector2f(1.0f, 1.0f)),
        Vertex(Vector3f( 1.0f,  1.0f,  1.0f), Vector2f(0.0f, 1.0f)),

        Vertex(Vector3f( 0.0f,  0.0f,  0.0f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f( 1.0f,  0.0f,  0.0f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f( 1.0f,  1.0f,  0.0f), Vector2f(1.0f, 1.0f)),
        Vertex(Vector3f( 0.0f,  1.0f,  0.0f), Vector2f(0.0f, 1.0f)),

        Vertex(Vector3f( 0.0f,  0.0f,  1.0f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f( 0.0f,  0.0f,  0.0f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f( 0.0f,  1.0f,  0.0f), Vector2f(1.0f, 1.0f)),
        Vertex(Vector3f( 0.0f,  1.0f,  1.0f), Vector2f(0.0f, 1.0f)),

        Vertex(Vector3f( 1.0f,  0.0f,  0.0f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f( 1.0f,  0.0f,  1.0f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f( 1.0f,  1.0f,  1.0f), Vector2f(1.0f, 1.0f)),
        Vertex(Vector3f( 1.0f,  1.0f,  0.0f), Vector2f(0.0f, 1.0f)),

        Vertex(Vector3f( 0.0f,  1.0f,  0.0f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f( 1.0f,  1.0f,  0.0f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f( 1.0f,  1.0f,  1.0f), Vector2f(1.0f, 1.0f)),
        Vertex(Vector3f( 0.0f,  1.0f,  1.0f), Vector2f(0.0f, 1.0f)),

        Vertex(Vector3f( 1.0f,  0.0f,  0.0f), Vector2f(0.0f, 0.0f)),
        Vertex(Vector3f( 0.0f,  0.0f,  0.0f), Vector2f(1.0f, 0.0f)),
        Vertex(Vector3f( 0.0f,  0.0f,  1.0f), Vector2f(1.0f, 1.0f)),
        Vertex(Vector3f( 1.0f,  0.0f,  1.0f), Vector2f(0.0f, 1.0f))
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    calculateNormals(indices, 36, vertices, 24);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int width, height;
    image = loadPNG("stone.png", &width, &height);
    assert(image != 0);
}*/

/*void Block::Render(Shader* shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    shader->SetVertexData();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}*/
