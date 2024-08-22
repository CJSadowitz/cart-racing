#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

class Mesh
{
public:
    Mesh(std::string file_path);
    void set_texture(std::string file_path);

    int get_indices_size();
    int get_vertices_size();

    std::vector<int> get_indices_vector();
    std::vector<float> get_vertices_vector();

private:
    std::vector<float> vertices;
    std::vector<int> indices;

    std::string texture_path;
};

#endif