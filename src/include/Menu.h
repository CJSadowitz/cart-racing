#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


class Menu
{
public:
    Menu(size_t layers);
    void generate(size_t index, float* vertices, size_t size);
    void bind(size_t index);
    void unbind();

private:
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
};

#endif