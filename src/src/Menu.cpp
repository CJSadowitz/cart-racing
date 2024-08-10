#include "Menu.h"
#include <iostream>

/*
Handle the packing of values into the buffer:
Currently just position. No normals, textures, color etc
*/
void Menu::generate(float* vertices, size_t size)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

GLuint Menu::get_VAO()
{
    return this->VAO;
}

GLuint Menu::get_VBO()
{   
    return this->VBO;
}

Menu::Menu()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
}

void Menu::bind()
{
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
}

void Menu::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
