#ifndef BUTTON_H
#define BUTTON_H

#include "Input.h"
#include <string>
#include <vector>

class Button
{
public:
    Button(std::string file_path);
    std::string clicked(GLFWwindow* window, mouse_pos mouse, int current_state, int previous_state);

    void generate_buffer(std::string file_path);

    std::vector<float> get_vertices_vector();
    std::vector<int> get_indices_vector();

    int get_indices_size();
    float get_vertices_size();

private:
    std::string texture_path;
    std::vector<float> vertices;
    std::vector<int> indices;
};

#endif