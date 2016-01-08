#ifndef MESH_HPP
#define MESH_HPP

class Shader;

class Mesh
{
public:
    virtual void Render(Shader* shader);

protected:
    unsigned int VBO;
    unsigned int IBO;
};

#endif //MESH_HPP
