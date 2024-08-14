#ifndef MENU_BUILDER_H
#define MENU_BUILDER_H

#include "Menu.h"
#include "GUI_Util.h"

class Menu_Builder
{
public:
    static void main_menu_builder(Menu& menu, GUI_Util& gui_util, size_t& size);
    static void settings_menu_builder(Menu& menu, GUI_Util& gui_util, size_t& size);

private:

};

#endif