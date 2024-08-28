#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"

class Button
{
public:
    Button(std::string file_path);
    bool clicked(GLFWwindow* window, mouse_pos mouse);
    void set_button_rect_positions(glm::mat4& transform);

    void generate_buffer(std::string file_path);

    std::vector<float> get_vertices_vector();
    std::vector<int> get_indices_vector();

    int get_indices_size();
    float get_vertices_size();

private:
    std::string name;

    std::vector<float> b_left;
    std::vector<float> t_right;

    std::vector<float> vertices;
    std::vector<int> indices;
};

#endif