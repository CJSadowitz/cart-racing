#include "Menu.h"
#include <iostream>

Menu::Menu(size_t layers)
{
    VAOs.resize(layers);
    VBOs.resize(layers);
    glGenVertexArrays(layers, VAOs.data());
    glGenBuffers(layers, VBOs.data());
}

void Menu::generate(size_t index, float* vertices, size_t size)
{
    bind(index);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Menu::bind(size_t index)
{
    glBindVertexArray(VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[index]);
}

void Menu::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}