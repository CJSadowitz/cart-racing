#ifndef GUI_UTIL_H
#define GUI_UTIL_H

#include <string>
#include "Input.h"
#include <vector>
#include <array>

struct rect_positions {
    float* positions;
    float* color;
};

struct sprite_positions {
    float* positions;
    std::string texture;
};

class GUI_Util
{
public:
    // Square Buttons
    rect_positions set_rect_positions(float* b_left, float* t_right, float* color);
    sprite_positions set_sprite_positions(float* b_left, float* t_right, float* texture);

    sprite_positions set_background(float* b_left, float* t_right, std::string texture);

    // returns the index of the button
    int is_pressed(GLFWwindow* window, mouse_pos mouse);

private:
    std::vector<std::array<float, 4>> button_pos;
    
};

#endif