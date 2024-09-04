#include "Display.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Display::Display(const std::string& file_path)
{
    std::ifstream file(file_path);
    std::string line;

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> textures;
    std::vector<int> indices;

    std::vector<int> normalIndices;
    std::vector<int> textureIndices;

    if (!file.is_open())
    {
        std::cout << "Display: " << file_path << "Could_Not_Open_File" << std::endl;
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
                int vertexIndex, textureIndex, normalIndex;
                segment_stream >> vertexIndex >> textureIndex >> normalIndex;

                indices.push_back(vertexIndex - 1);
                textureIndices.push_back(textureIndex - 1);
                normalIndices.push_back(normalIndex - 1);
            }
        }
    }
    std::vector<int> quad_indices;

    for (int i = 0; i < indices.size(); i++)
    {
        quad_indices.push_back(indices[i]);
        this->vertices.push_back(vertices[indices[i] * 3]);
        this->vertices.push_back(vertices[indices[i] * 3 + 1]);
        this->vertices.push_back(vertices[indices[i] * 3 + 2]);

        // this->vertices.push_back(normals[normalIndices[i] * 3]);
        // this->vertices.push_back(normals[normalIndices[i] * 3 + 1]);
        // this->vertices.push_back(normals[normalIndices[i] * 3 + 2]);

        this->vertices.push_back(textures[textureIndices[i] * 2]);
        this->vertices.push_back(textures[textureIndices[i] * 2 + 1]);
    }

    for (int i = 0; i < quad_indices.size(); i += 4)
    {
        this->indices.push_back(i + 0);
        this->indices.push_back(i + 1);
        this->indices.push_back(i + 2);

        this->indices.push_back(i + 0);
        this->indices.push_back(i + 2);
        this->indices.push_back(i + 3);
    }
    // std::cout << "\nVertices: " << std::endl;
    // for (int i = 0; i < this->vertices.size(); i++)
    // {
    //     if (i != 0 && i % 8 == 0)
    //     {
    //         std::cout << std::endl;
    //     }
    //     std::cout << this->vertices[i] << ' ';
    // }
    // std::cout << std::endl;   
    // std::cout << "Incides:" << std::endl;
    // for (int i = 0; i < this->indices.size(); i++)
    // {
    //     std::cout << this->indices[i] << ' ';
    // }
    // std::cout << std::endl;
}

int Display::get_indices_size()
{
    return this->indices.size();
}

float Display::get_vertices_size()
{
    return this->vertices.size();
}

const std::vector<int>& Display::get_indices_vector()
{
    return this->indices;
}

const std::vector<float>& Display::get_vertices_vector()
{
    return this->vertices;
}
