#include "chunkmanager.hpp"

#include "chunk.hpp"
#include "vector.hpp"

#include <vector>

namespace ChunkManager
{
    std::vector<Chunk*> chunks;
    Chunk* getChunk(int chunkX, int chunkZ)
    {
        return chunks[chunkX * WORLD_SIZE_X + chunkZ];
    }

    Chunk* getChunkFromBlock(int x, int y, int z)
    {
        return getChunk(x / 16, z / 16);
    }

    void getSurroundingChunks(int chunkX, int chunkZ, Chunk*& left, Chunk*& right, Chunk*& front, Chunk*& back)
    {
        left = right = front = back = nullptr;

        if (chunkX > 0)
            left = getChunk(chunkX - 1, chunkZ);
        if (chunkX < WORLD_SIZE_X - 1)
            right = getChunk(chunkX + 1, chunkZ);
        if (chunkZ > 0)
            back  = getChunk(chunkX, chunkZ - 1);
        if (chunkZ < WORLD_SIZE_Z - 1)
            front = getChunk(chunkX, chunkZ + 1);
    }

    void getChunkDataForBlock(int x, int y, int z, Chunk*& chunk, Chunk*& left, Chunk*& right, Chunk*& front, Chunk*& back)
    {
        int chunkX = x/16;
        int chunkZ = z/16;
        chunk = chunks[chunkX * WORLD_SIZE_X + chunkZ];
        getSurroundingChunks(chunkX, chunkZ, left, right, front, back);
    }

    void bufferGraphicsData()
    {
        for (int i = 0; i < WORLD_SIZE_X * WORLD_SIZE_Z; i++)
            chunks.push_back(new Chunk());

        for (int x = 0; x < WORLD_SIZE_X; x++)
        {
            for (int z = 0; z < WORLD_SIZE_Z; z++)
            {
                Chunk *left, *right, *front, *back;
                getSurroundingChunks(x, z, left, right, front, back);

                int chunkIndex = x * WORLD_SIZE_X + z;
                chunks[chunkIndex]->CreateMesh(left, right, front, back);
            }
        }
    }

    bool raycast(const Vector3f& start, const Vector3f& dir, Vector3f& hit, Side& side)
    {
        // TODO: Change this so it always steps to the next block instead of a fixed distance
        float dist = 0.1f;

        int lastX = 0, lastY = 0, lastZ = 0;
        while (dist < 5.0f)
        {
            Vector3f v = start + dir * dist;
            int x = v.x;
            int y = v.y;
            int z = v.z;

            if (x >= 0 && z >= 0 && y > 0 && x < WORLD_SIZE_X * 16 && z < WORLD_SIZE_Z * 16 && y < 256)
            {
                int chunkIndex = (x / 16) * WORLD_SIZE_X + (z / 16);
                if (chunks[chunkIndex]->IsBlockSolid(x % 16, y, z % 16))
                {
                    hit = Vector3f(x, y, z);

                    // Find out which side that was hit
                    if (lastX != x)
                        side = lastX < x ? Left : Right;
                    else if (lastY != y)
                        side = lastY < y ? Bottom : Top;
                    else if (lastZ != z)
                        side = lastZ < z ? Front : Back;
                    return true;
                }
            }

            lastX = x;
            lastY = y;
            lastZ = z;

            dist += 0.1f;
        }

        return false;
    }

    void rebuildChunk(int chunkX, int chunkZ)
    {
        Chunk *left, *right, *front, *back;
        getSurroundingChunks(chunkX, chunkZ, left, right, front, back);

        int chunkIndex = chunkX * WORLD_SIZE_X + chunkZ;
        chunks[chunkIndex]->CreateMesh(left, right, front, back);
    }

    void rebuildNeighbours(int x, int z)
    {
        int chunkX = x / 16;
        int chunkZ = z / 16;
        int localX = x % 16;
        int localZ = z % 16;

        if (localX == 0 && chunkX > 0)
            rebuildChunk(chunkX - 1, chunkZ);
        if (localX == 15 && chunkX < WORLD_SIZE_X - 1)
            rebuildChunk(chunkX + 1, chunkZ);
        if (localZ == 0 && chunkZ > 0)
            rebuildChunk(chunkX, chunkZ - 1);
        if (localZ == 15 && chunkZ < WORLD_SIZE_Z - 1)
            rebuildChunk(chunkX, chunkZ + 1);
    }

    // TODO: Segfault when trying to add block outside world border
    // (only in one direction) check raycast code
    void addBlock(int x, int y, int z)
    {
        Chunk *chunk, *left, *right, *front, *back;
        getChunkDataForBlock(x, y, z, chunk, left, right, front, back);
        chunk->PlaceBlock(x % 16, y, z % 16, left, right, front, back);
        rebuildNeighbours(x, z);
    }

    // TODO: Check world borders
    void removeBlock(int x, int y, int z)
    {
        Chunk *chunk, *left, *right, *front, *back;
        getChunkDataForBlock(x, y, z, chunk, left, right, front, back);
        chunk->RemoveBlock(x % 16, y, z % 16, left, right, front, back);
        rebuildNeighbours(x, z);
    }
}
