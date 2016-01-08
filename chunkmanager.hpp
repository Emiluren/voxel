#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

class Chunk;
class Vector3f;

enum Side {
    Top,
    Bottom,
    Left,
    Right,
    Front,
    Back
};

namespace ChunkManager
{
    const int WORLD_SIZE_X = 4;
    const int WORLD_SIZE_Z = 4;

    Chunk* getChunk(int chunkX, int chunkZ);
    Chunk* getChunkFromBlock(int x, int y, int z);
    void getSurroundingChunks(int chunkX, int chunkZ, Chunk*& left, Chunk*& right, Chunk*& front, Chunk*& back);
    void getChunkDataForBlock(int x, int y, int z, Chunk*& chunk, Chunk*& left, Chunk*& right, Chunk*& front, Chunk*& back);
    void bufferGraphicsData();
    bool raycast(const Vector3f& start, const Vector3f& dir, Vector3f& hit, Side& side);
    void rebuildChunk(int chunkX, int chunkZ);
    void rebuildNeighbours(int x, int z);
    void addBlock(int x, int y, int z);
    void removeBlock(int x, int y, int z);
}

#endif // CUNKMANAGER_HPP
