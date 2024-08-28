#include "Button.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem> 

Button::Button(std::string file_path)
{
    for (const auto& entry: std::filesystem::directory_iterator(file_path))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            // create a button for each button directory in the buttons folder
            auto mesh_path = entry.path();
            generate_buffer(mesh_path);
        }
    }
}

void Button::generate_buffer(std::string file_path)
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
        std::cout << "Button: Could_Not_Open_File" << std::endl;
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

void Button::set_button_rect_positions(glm::mat4& transform)
{
    // Vertex Array:
    // x1 y1 z1 t1u t1v, x2 y2 z2 t2u t2v, x3 y3 z3 t3u t3v, x4 y4 z4 t4u t4v,  
    float x_min = std::min({this->vertices[0], this->vertices[5], this->vertices[10], this->vertices[15]});
    float y_min = std::min({this->vertices[2], this->vertices[7], this->vertices[12], this->vertices[17]});

    float x_max = std::max({this->vertices[0], this->vertices[5], this->vertices[10], this->vertices[15]});
    float y_max = std::max({this->vertices[2], this->vertices[7], this->vertices[12], this->vertices[17]});

    glm::vec4 b_left = transform * glm::vec4(x_min, 0.0f, y_min, 1.0f);
    glm::vec4 t_right = transform * glm::vec4(x_max, 0.0f, y_max, 1.0f);

    this->b_left.push_back(b_left.x);
    this->b_left.push_back(b_left.y);
    this->b_left.push_back(0.0f);

    this->t_right.push_back(t_right.x);
    this->t_right.push_back(t_right.y);
    this->t_right.push_back(0.0f);
}

bool Button::clicked(GLFWwindow* window, mouse_pos mouse)
{
    Input mouse_fix;
    mouse_fix.update_mouse_pos(window, mouse);

    mouse_pos screen_mouse = mouse_fix.get_mouse_pos();

    bool x_bool = this->b_left[0] <= screen_mouse.x_pos && this->t_right[0] >= screen_mouse.x_pos;
    bool y_bool = this->b_left[1] <= screen_mouse.y_pos && this->t_right[1] >= screen_mouse.y_pos;
    if (x_bool && y_bool)
    {
        return true;
    }
    return false;
}

int Button::get_indices_size()
{
    return this->indices.size();
}

float Button::get_vertices_size()
{
    return this->vertices.size();
}

std::vector<int> Button::get_indices_vector()
{
    return this->indices;
}

std::vector<float> Button::get_vertices_vector()
{
    return this->vertices;
}
