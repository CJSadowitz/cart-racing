#include "Input.h"
#include <iostream>

void Input::update_mouse_pos(GLFWwindow *window, mouse_pos mouse)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    //Screen coords
    float x = (mouse.x_pos / width) * 2 - 1;
    float y = ((mouse.y_pos / height) * 2 - 1) * -1;

    mouse_pos new_mouse;
    new_mouse.x_pos = x;
    new_mouse.y_pos = y;

    this->mouse = new_mouse;
}

mouse_pos Input::get_mouse_pos()
{
    return this->mouse;
}