#ifndef BLOCK_HPP
#define BLOCK_HPP

enum BlockType
{
    Air,
    Stone,
    Dirt
};

class Block
{
public:
    Block() : type(Air) {}
    Block(BlockType type) : type(type) {}
    bool isSolid() { return type != Air; }

private:
    BlockType type;
};

#endif //BLOCK_HPP
