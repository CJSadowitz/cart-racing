#ifndef MESH_H
#define MESH_H

#include <string>

class Mesh
{
public:
    Mesh(std::string file_path);
    void set_texture(std::string file_path);

private:
    std::vector<float> vertices;
    std::vector<int> indices;

    std::string texture_path;
};

#endif