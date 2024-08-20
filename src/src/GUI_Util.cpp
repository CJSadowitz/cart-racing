#include "GUI_Util.h"

// Pass in x,y coords for b_left and t_right
rect_positions GUI_Util::set_rect_positions(float* b_left, float* t_right, float* color)
{
    // referenced for button logic later
    this->button_pos.push_back({b_left[0], b_left[1], t_right[0], t_right[1]});

    // generate solid color rectangle
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

sprite_positions GUI_Util::set_sprite_positions(float* b_left, float* t_right, std::string texture)
{
    // referenced for button logic later
    this->button_pos.push_back({b_left[0], b_left[1], t_right[0], t_right[1]});

    float* vertices = new float[30] {
        b_left[0], b_left[1],   0,    0, 0,
        t_right[0], b_left[1],  0,    1, 0,
        t_right[0], t_right[1], 0,    1, 1,

        b_left[0], b_left[1],   0,    0, 0,
        b_left[0], t_right[1],  0,    0, 1,
        t_right[0], t_right[1], 0,    1, 1
    };
    sprite_positions rectangle;
    rectangle.positions = vertices;
    rectangle.texture = texture;

    return rectangle;
}

int GUI_Util::is_hover(GLFWwindow* window, mouse_pos mouse)
{
    Input mouse_fix;
    mouse_fix.update_mouse_pos(window, mouse);

    mouse_pos screen_mouse = mouse_fix.get_mouse_pos();

    bool x_pos;
    bool y_pos;

    int i = 0;
    
    // check if mouse position is inside of a defined rectangle position
    for (const auto& row : this->button_pos)
    {
        x_pos = row[0] <= screen_mouse.x_pos && row[2] >= screen_mouse.x_pos;
        y_pos = row[1] <= screen_mouse.y_pos && row[3] >= screen_mouse.y_pos;

        if (x_pos && y_pos)
        {
            return i;
        }
        i++;
    }
    return -1;
}

