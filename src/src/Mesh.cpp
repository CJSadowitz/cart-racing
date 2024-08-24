#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Mesh::Mesh(std::string file_path)
{
    std::ifstream file(file_path);
    std::string line;

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> textures;
    std::vector<int> indices;

    if (!file.is_open())
    {
        std::cout << "MESH::Could_Not_Open_File" << std::endl;
        return;
    }

    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") // vertex
        {
            float value;
            while (ss >> value)
            {
                vertices.push_back(value);
            }
        }
        else if (prefix == "vn") // normals
        {
            float value;
            while (ss >> value)
            {
                normals.push_back(value);
            }
        }
        else if (prefix == "vt") // texture
        {
            float value;
            while (ss >> value)
            {
                textures.push_back(value);
            }
        }
        else if (prefix == "f")
        {
            std::string segment;
            while (ss >> segment)
            {
                std::replace(segment.begin(), segment.end(), '/', ' ');
                std::stringstream segment_stream(segment);
                int value;
                while (segment_stream >> value)
                {
                    indices.push_back(value - 1);
                }
            }
        }
    }
    std::vector<int> quad_indices;

    for (int i = 0; i < indices.size() / 3; i++)
    {
        quad_indices.push_back(indices[i * 3]);
        this->vertices.push_back(vertices[indices[i * 3] * 3]);
        this->vertices.push_back(vertices[indices[i * 3] * 3 + 1]);
        this->vertices.push_back(vertices[indices[i * 3] * 3 + 2]);
    
        this->vertices.push_back(normals[indices[i * 3 + 1] * 3]);
        this->vertices.push_back(normals[indices[i * 3 + 1] * 3 + 1]);
        this->vertices.push_back(normals[indices[i * 3 + 1] * 3 + 2]);
        
        this->vertices.push_back(textures[indices[i * 2 + 2] * 2]);
        this->vertices.push_back(textures[indices[i * 2 + 2] * 2 + 1]);
    }

    // for (int i = 0; i < quad_indices.size(); i += 4)
    // {
    //     this->indices.push_back(quad_indices[i + 0]);
    //     this->indices.push_back(quad_indices[i + 1]);
    //     this->indices.push_back(quad_indices[i + 2]);

    //     this->indices.push_back(quad_indices[i + 0]);
    //     this->indices.push_back(quad_indices[i + 2]);
    //     this->indices.push_back(quad_indices[i + 3]);
    // }
    for (int i = 0; i < quad_indices.size(); i += 4)
    {
        this->indices.push_back(i + 0);
        this->indices.push_back(i + 1);
        this->indices.push_back(i + 2);

        this->indices.push_back(i + 0);
        this->indices.push_back(i + 2);
        this->indices.push_back(i + 3);
    }
}

void Mesh::set_texture(std::string file_path)
{
    this->texture_path == file_path;
}

int Mesh::get_indices_size()
{
    return this->indices.size();
}

int Mesh::get_vertices_size()
{
    return this->vertices.size();
}

std::vector<float> Mesh::get_vertices_vector()
{
    return this->vertices;
}

std::vector<int> Mesh::get_indices_vector()
{
    return this->indices;
}
