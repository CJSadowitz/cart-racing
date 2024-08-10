#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <string>
#include "Menu.h"

class State_Machine
{
public:

    State_Machine();
    // function called in loop
    void run_state();

    void set_state(std::string state);
    std::string get_state();

    void main_menu_state();
    void settings_menu_state();
    void game_state();

private:
    bool first_call;
    std::string state;

    unsigned int VAO;
    unsigned int VBO;

    Menu menu_obj;
};

#endif