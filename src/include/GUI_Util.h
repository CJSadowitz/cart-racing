#ifndef GUI_UTIL_H
#define GUI_UTIL_H

#include <string>

struct Sprite_Button
{
    float* position;
    char* sprite_path;
};

struct Rect_Button
{
    float* position;
};

class GUI_Util
{
public:
    // Gather all information
    void set_sprite_button_positions(Sprite_Button* buttons);
    void set_rect_button_positions(Rect_Button* buttons);

    void set_background(float* positions, std::string array);

    // Send out all information
    float* get_positions();
    std::string* get_textures();

private:
    float* positions;
    std::string* textures;
    
};

#endif