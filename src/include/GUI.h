#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Button.h"
#include "Display.h"
#include "Shader.h"
#include <string>
#include <vector>

class Gui
{
public:
    Gui(std::string file_path);
    void generate_buttons(int index);
    void generate_displays(int index);
    void bind(int index);
    void unbind();

    void render();

    std::vector<Button> get_buttons();
    std::vector<Display> get_displays();

private:
    std::vector<Shader> button_shaders;
    std::vector<Shader> display_shaders;

    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<GLuint> EBOs;
    std::vector<GLuint> textures;

    std::vector<Button> buttons;
    std::vector<Display> displays;
};

#endif