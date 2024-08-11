#ifndef GUI_UTIL_H
#define GUI_UTIL_H

#include <string>

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

    bool is_pressed(rect_positions button, float* mouse_pos);
    bool is_pressed(sprite_positions button, float* mouse_pos);


private:
    
    
};

#endif