#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Menu
{
public:

    Menu();
    // generate all information related to the main_menu
    void generate(float* vertices, size_t size);

    void bind();
    void unbind();

private:
    float* vertices;
    
    GLuint VAO;
    GLuint VBO;
};

#endif