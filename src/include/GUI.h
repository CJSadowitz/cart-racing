#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Button.h"
#include "Display.h"
#include <string>
#include <vector>

class Gui
{
public:
    Gui(std::string file_path);
    void generate_buttons();
    void generate_displays();

    void display_bind(int index);
    void button_bind(int index);
    void unbind();

    void render(GLFWwindow* window, mouse_pos mouse);

    int get_button_pressed();

    std::vector<Button> get_buttons();
    std::vector<Display> get_displays();

private:
    std::vector<std::string> display_file_path;
    std::vector<std::string> button_file_path;

    std::vector<std::string> button_texture_paths;
    std::vector<std::string> display_texture_paths;

    int button_index;

    glm::mat4 transform;

    std::vector<Shader> button_shaders;
    std::vector<Shader> display_shaders;

    std::vector<GLuint> B_VAOs;
    std::vector<GLuint> B_VBOs;
    std::vector<GLuint> B_EBOs;

    std::vector<GLuint> D_VAOs;
    std::vector<GLuint> D_VBOs;
    std::vector<GLuint> D_EBOs;

    std::vector<GLuint> button_textures;
    std::vector<GLuint> display_textures;

    std::vector<Button> buttons;
    std::vector<Display> displays;
};

#endif