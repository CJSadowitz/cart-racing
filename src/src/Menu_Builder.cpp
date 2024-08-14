#include "Menu_Builder.h"

void Menu_Builder::main_menu_builder(Menu& menu, GUI_Util& gui_util, size_t& size)
{
    // GUI creation
    float b_left1[] = {-0.875, -0.875};
    float t_right1[] = {-0.6, -0.6};
    float color1[] = {1.0, 0.0, 0.0};
    rect_positions rect1 = gui_util.set_rect_positions(b_left1, t_right1, color1);
    std::vector<float> all_pos(rect1.positions, rect1.positions + 36);
        
    float b_left2[] = {-0.875, -0.575};
    float t_right2[] = {-0.6, -0.2};
    float color2[] = {0.0, 1.0, 0.0};
    rect_positions rect2 = gui_util.set_rect_positions(b_left2, t_right2, color2);
    std::vector<float> button_two(rect2.positions, rect2.positions + 36);

    all_pos.insert(all_pos.end(), button_two.begin(), button_two.end());
        
    // Each button is 144 bytes
    size = all_pos.size() * sizeof(float);
    menu.generate(0, all_pos.data(), size);
        
    delete rect1.positions;
    delete rect2.positions;
}

void Menu_Builder::settings_menu_builder(Menu& menu, GUI_Util& gui_util, size_t& size)
{
    
}