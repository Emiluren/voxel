#include "vertex.hpp"

void calculateNormals(const unsigned int* indices, unsigned int indexCount,
                      Vertex* vertices, unsigned int vertexCount)
{
    for (unsigned int i = 0; i < indexCount; i += 3)
    {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i + 1];
        unsigned int index2 = indices[i + 2];

        Vector3f v1 = vertices[index0].position - vertices[index2].position;
        Vector3f v2 = vertices[index0].position - vertices[index1].position;
        Vector3f normal = v1.Cross(v2);
        normal.Normalize();

        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }

    for (unsigned int i = 0; i < vertexCount; i++)
    {
        vertices[i].normal.Normalize();
    }
}
