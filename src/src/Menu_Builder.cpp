#include "Menu_Builder.h"

/*
Depricated for GUI/file reader
*/

void Menu_Builder::main_menu_builder(Menu& menu, GUI_Util& gui_util, std::vector<size_t>& size)
{
    // GUI creation
    float b_left1[] = {-0.8, -0.8};
    float t_right1[] = {-0.3, -0.65};
    float color1[] = {1.0, 0.0, 0.0};
    rect_positions rect1 = gui_util.set_rect_positions(b_left1, t_right1, color1);
    std::vector<float> all_pos(rect1.positions, rect1.positions + 36);
        
    float b_left2[] = {-0.8, -0.4};
    float t_right2[] = {-0.3, -0.25};
    float color2[] = {0.0, 1.0, 0.0};
    rect_positions rect2 = gui_util.set_rect_positions(b_left2, t_right2, color2);
    std::vector<float> button_two(rect2.positions, rect2.positions + 36);

    all_pos.insert(all_pos.end(), button_two.begin(), button_two.end());
        
    // Each button is 144 bytes
    size.push_back(all_pos.size() * sizeof(float));
    menu.generate(0, all_pos.data(), all_pos.size() * sizeof(float));
        
    delete rect1.positions;
    delete rect2.positions;
}

void Menu_Builder::settings_menu_builder(Menu& menu, GUI_Util& gui_util, std::vector<size_t>& size)
{
    // GUI creation
    float b_left1[] = {-0.8, -0.4};
    float t_right1[] = {-0.4, -0.3};
    float color1[] = {1.0, 0.0, 0.0};
    rect_positions rect1 = gui_util.set_rect_positions(b_left1, t_right1, color1);
    std::vector<float> all_pos(rect1.positions, rect1.positions + 36);
        
    float b_left2[] = {-0.8, -0.8};
    float t_right2[] = {-0.4, -0.7};
    float color2[] = {0.5, 0.26, 0.49};
    rect_positions rect2 = gui_util.set_rect_positions(b_left2, t_right2, color2);
    std::vector<float> button_two(rect2.positions, rect2.positions + 36);

    all_pos.insert(all_pos.end(), button_two.begin(), button_two.end());
        
    // Each button is 144 bytes
    size.push_back(all_pos.size() * sizeof(float));
    menu.generate(0, all_pos.data(), all_pos.size() * sizeof(float));
        
    delete rect1.positions;
    delete rect2.positions;
}
