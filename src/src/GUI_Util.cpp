#include "GUI_Util.h"

// Pass in x,y coords for b_left and t_right
rect_positions GUI_Util::set_rect_positions(float* b_left, float* t_right, float* color)
{
    // generate the vertex data with color inserted (note that the color is solid)
    float* vertices = new float[36] {
        b_left[0], b_left[1],   0, color[0], color[1], color[2],
        t_right[0], b_left[1],  0, color[0], color[1], color[2],
        t_right[0], t_right[1], 0, color[0], color[1], color[2],

        b_left[0], b_left[1],   0, color[0], color[1], color[2],
        b_left[0], t_right[1],  0, color[0], color[1], color[2],
        t_right[0], t_right[1], 0, color[0], color[1], color[2],
    };
    rect_positions rectangle;
    rectangle.positions = vertices;
    rectangle.color = color;

    return rectangle;
}

// sprite_positions GUI_Util::set_sprite_positions(float* b_left, float* t_right, int* color)
// {
    
// }

// sprite_positions GUI_Util::set_background(float* b_left, float* t_right, std::string texture)
// {

// }

// bool GUI_Util::is_pressed(rect_positions button, float* mouse_pos)
// {

// }

// bool GUI_Util::is_pressed(sprite_positions button, float* mouse_pos)
// {

// }
