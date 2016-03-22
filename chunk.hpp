#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

class Block;
class Shader;
struct Vertex;

class Chunk
{
public:
    Chunk();
    ~Chunk();

    void PlaceBlock(int x, int y, int z, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk);
    void RemoveBlock(int x, int y, int z, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk);
    bool IsBlockSolid(int x, int y, int z);
    void CreateMesh(Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk);
    void Render(Shader* shader);

    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 256;

private:
    void CreateCube(int x, int y, int z, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk);

    Block*** blocks;
    unsigned int image;
    unsigned int VAO, VBO, IBO;
    std::vector<Vertex>* mesh_builder;
    std::vector<unsigned int>* tri_builder;
};

#endif // CHUNK_HPP
