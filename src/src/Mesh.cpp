#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Mesh::Mesh(const std::string& file_path)
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
        std::cout << "Mesh: Could_Not_Open_File" << std::endl;
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
    std::vector<vertex> verts;
    for (int i = 0; i < indices.size(); i++)
    {
        vertex new_vert;
        quad_indices.push_back(indices[i]);
        this->vertices.push_back(vertices[indices[i] * 3]);
        this->vertices.push_back(vertices[indices[i] * 3 + 1]);
        this->vertices.push_back(vertices[indices[i] * 3 + 2]);
        new_vert.pos = glm::vec3(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        verts.push_back(new_vert);

        this->vertices.push_back(normals[normalIndices[i] * 3]);
        this->vertices.push_back(normals[normalIndices[i] * 3 + 1]);
        this->vertices.push_back(normals[normalIndices[i] * 3 + 2]);

        this->vertices.push_back(textures[textureIndices[i] * 2]);
        this->vertices.push_back(textures[textureIndices[i] * 2 + 1]);
    }

    for (int i = 0; i < quad_indices.size(); i += 4)
    {
        tri first_tri;
        this->indices.push_back(i + 0);
        this->indices.push_back(i + 1);
        this->indices.push_back(i + 2);

        first_tri.vertex0 = verts[i + 0];
        first_tri.vertex1 = verts[i + 1];
        first_tri.vertex2 = verts[i + 2];
        this->triangles.push_back(first_tri);

        tri second_tri;
        this->indices.push_back(i + 0);
        this->indices.push_back(i + 2);
        this->indices.push_back(i + 3);

        second_tri.vertex0 = verts[i + 0];
        second_tri.vertex1 = verts[i + 2];
        second_tri.vertex2 = verts[i + 3];
        this->triangles.push_back(second_tri);
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

    // for (int i = 0; i < this->vertices.size() / 3; i += 8)
    // {
    //     tri new_tri;
    //     new_tri.vertex0 = glm::vec3(this->vertices[i], this->vertices[i + 1], this->vertices[i + 2]);
    //     new_tri.vertex1 = glm::vec3(this->vertices[i + 8], this->vertices[i + 9], this->vertices[i + 10]);
    //     new_tri.vertex2 = glm::vec3(this->vertices[i + 16], this->vertices[i + 17], this->vertices[i + 18]);
    //     // // std::cout << "Vertices size: " << this->vertices.size() << " furthest read: " << i + 18 << std::endl;
    //     // // std::cout << "TRIANGLE VALUES ____________________________________" << std::endl;
    //     // // std::cout << "x: " << new_tri.vertex0.x << " y: " << new_tri.vertex0.y << " z: " << new_tri.vertex0.z << std::endl;
    //     // // std::cout << "x: " << new_tri.vertex1.x << " y: " << new_tri.vertex1.y << " z: " << new_tri.vertex1.z << std::endl;
    //     // // std::cout << "x: " << new_tri.vertex2.x << " y: " << new_tri.vertex2.y << " z: " << new_tri.vertex2.z << std::endl;
    //     // // std::cout << "TRIANGLE VALUES ____________________________________" << std::endl;
    //     this->triangles.push_back(new_tri);
    // }
    // I need to generate 12 triangles from 8 vertices given an array of 24 float values. Describing xyz
}

void Mesh::set_texture(const std::string& file_path)
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

const std::vector<float>& Mesh::get_vertices_vector()
{
    return this->vertices;
}

const std::vector<int>& Mesh::get_indices_vector()
{
    return this->indices;
}
