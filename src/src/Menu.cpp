#include "Menu.h"

/*
Handle the packing of values into the buffer:
Currently just position. No normals, textures, color etc
*/
void Menu::generate(float* vertices)
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    this->VAO = VAO;
    this->VBO = VBO;
}

unsigned int Menu::get_VAO()
{
    return this->VAO;
}

unsigned int Menu::get_VBO()
{   
    return this->VBO;
}

std::string Menu::get_texture()
{
    return this->texture;
}
