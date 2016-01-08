#include "chunk.hpp"
#include "block.hpp"
#include "vertex.hpp"
#include "lib.h"
#include "shader.hpp"

#include <cassert>
#include <iostream>

Chunk::Chunk()
{
    blocks = new Block**[CHUNK_SIZE];
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        blocks[x] = new Block*[CHUNK_HEIGHT];
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            blocks[x][y] = new Block[CHUNK_SIZE];
        }
    }

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 60; y++)
        {
            for (int z = 0; z < 16; z++)
            {
                blocks[x][y][z] = Stone;
            }
        }
    }
    mesh_builder = new std::vector<Vertex>();
    tri_builder = new std::vector<unsigned int>();

    int width, height;
    image = loadPNG("stone.png", &width, &height);
    assert(image != 0);
}

Chunk::~Chunk()
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            delete[] blocks[x][y];
        }
        delete[] blocks[x];
    }
    delete[] blocks;
    delete mesh_builder;
    delete tri_builder;
}

void Chunk::PlaceBlock(int x, int y, int z, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk)
{
    blocks[x][y][z] = Stone;
    CreateMesh(leftChunk, rightChunk, frontChunk, backChunk);
    // TODO: Rebuild mesh of neighbouring chunk
}

void Chunk::RemoveBlock(int x, int y, int z, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk)
{
    blocks[x][y][z] = Air;
    CreateMesh(leftChunk, rightChunk, frontChunk, backChunk);
    // TODO: Rebuild mesh of neighbouring chunk
}

bool Chunk::IsBlockSolid(int x, int y, int z)
{
    return blocks[x][y][z].isSolid();
}

void Chunk::CreateMesh(Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk)
{
    delete mesh_builder;
    delete tri_builder;
    mesh_builder = new std::vector<Vertex>();
    tri_builder = new std::vector<unsigned int>();

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blocks[x][y][z].isSolid())
                {
                    CreateCube(x, y, z, leftChunk, rightChunk, frontChunk, backChunk);
                }
            }
        }
    }

    calculateNormals(tri_builder->data(), tri_builder->size(), mesh_builder->data(), mesh_builder->size());

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh_builder->size() * sizeof(Vertex),
                 mesh_builder->data(), GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_builder->size() * sizeof(unsigned int),
                 tri_builder->data(), GL_STATIC_DRAW);
}

void Chunk::CreateCube(int x, int y, int z, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk)
{
    // All corners of the cube
    Vertex v1(Vector3f(x+1, y+0, z+1));
    Vertex v2(Vector3f(x+0, y+0, z+1));
    Vertex v3(Vector3f(x+0, y+1, z+1));
    Vertex v4(Vector3f(x+1, y+1, z+1));

    Vertex v5(Vector3f(x+0, y+0, z+0));
    Vertex v6(Vector3f(x+1, y+0, z+0));
    Vertex v7(Vector3f(x+1, y+1, z+0));
    Vertex v8(Vector3f(x+0, y+1, z+0));

    Vertex vertices[] = {
        v1, v2, v3, v4, v5, v6, v7, v8,
        v2, v5, v8, v3, v6, v1, v4, v7,
        v8, v7, v4, v3, v6, v5, v2, v1
    };

    for (int i = 0; i < 6; i++)
    {
        vertices[i*4].uv   = Vector2f(0, 0);
        vertices[i*4+1].uv = Vector2f(1, 0);
        vertices[i*4+2].uv = Vector2f(1, 1);
        vertices[i*4+3].uv = Vector2f(0, 1);
    }

    GLuint indices[] = {
        0,  1,  2,  0,  2,  3
    };

    int faces = 0;
    unsigned int offset = mesh_builder->size();

    if (z == 15 || !blocks[x][y][z+1].isSolid()) // Front face
    {
        if (z != 15 || frontChunk == nullptr || !frontChunk->IsBlockSolid(x,y,0))
        {
            for (int i = 0; i < 4; i++)
                mesh_builder->push_back(vertices[i]);

            for (int i = 0; i < 6; i++)
                tri_builder->push_back(indices[i] + offset);

            offset += 4;
            faces++;
        }
    }

    if (z == 0 || !blocks[x][y][z-1].isSolid()) // Back face
    {
        if (z != 0 || backChunk == nullptr || !backChunk->IsBlockSolid(x,y,15))
        {
            for (int i = 4; i < 8; i++)
                mesh_builder->push_back(vertices[i]);

            for (int i = 0; i < 6; i++)
                tri_builder->push_back(indices[i] + offset);

            offset += 4;
            faces++;
        }
    }

    if (x == 0 || !blocks[x-1][y][z].isSolid()) // Left face
    {
        if (x != 0 || leftChunk == nullptr || !leftChunk->IsBlockSolid(15,y,z))
        {
            for (int i = 8; i < 12; i++)
                mesh_builder->push_back(vertices[i]);

            for (int i = 0; i < 6; i++)
                tri_builder->push_back(indices[i] + offset);

            offset += 4;
            faces++;
        }
    }

    if (x == 15 || !blocks[x+1][y][z].isSolid()) // Right face
    {
        if (x != 15 || rightChunk == nullptr || !rightChunk->IsBlockSolid(0,y,z))
        {
            for (int i = 12; i < 16; i++)
                mesh_builder->push_back(vertices[i]);

            for (int i = 0; i < 6; i++)
                tri_builder->push_back(indices[i] + offset);

            offset += 4;
            faces++;
        }
    }

    if (y == 255 || !blocks[x][y+1][z].isSolid()) // Top face
    {
        for (int i = 16; i < 20; i++)
            mesh_builder->push_back(vertices[i]);

        for (int i = 0; i < 6; i++)
            tri_builder->push_back(indices[i] + offset);

        offset += 4;
        faces++;
    }

    if (y == 0 || !blocks[x][y-1][z].isSolid()) // Bottom face
    {
        for (int i = 20; i < 24; i++)
            mesh_builder->push_back(vertices[i]);

        for (int i = 0; i < 6; i++)
            tri_builder->push_back(indices[i] + offset);

        offset += 4;
        faces++;
    }
}

void Chunk::Render(Shader* shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    shader->SetVertexData();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, tri_builder->size(), GL_UNSIGNED_INT, 0);
}
