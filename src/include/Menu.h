#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Menu
{
public:
    // generate all information related to the main_menu
    void generate(float* vertices);

    unsigned int get_VAO();
    unsigned int get_VBO();

    std::string get_texture();

private:
    float* vertices;
    std::string texture;
    
    unsigned int VAO;
    unsigned int VBO;
};

#endif