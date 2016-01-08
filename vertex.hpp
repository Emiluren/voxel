#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "vector.hpp"

struct Vertex
{
    Vector3f position;
    Vector2f uv;
    Vector3f normal;

    Vertex() {}

    Vertex(Vector3f pos) { this->position = pos; }

    Vertex(Vector3f pos, Vector2f uv, Vector3f norm)
    {
        this->position = pos;
        this->uv = uv;
        this->normal = norm;
    }

    Vertex(Vector3f pos, Vector3f norm)
    {
        this->position = pos;
        this->normal = norm;
    }

    Vertex(Vector3f pos, Vector2f uv)
    {
        this->position = pos;
        this->uv = uv;
    }
};

void calculateNormals(const unsigned int* indices, unsigned int indexCount,
                      Vertex* vertices, unsigned int vertexCount);

#endif // VERTEX_HPP
