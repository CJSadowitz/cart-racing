#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <string>
#include "Menu.h"
#include "Shader.h"
#include "GUI_Util.h"
#include "Input.h"
#include <GLFW/glfw3.h>

class State_Machine
{
public:

    State_Machine();
    // function called in loop
    void run_state(GLFWwindow* window, mouse_pos mouse);

    void set_state(std::string state);
    std::string get_state();

    void main_menu_state(GLFWwindow* window, mouse_pos mouse);
    void settings_menu_state(GLFWwindow* window, mouse_pos mouse);
    void game_state(GLFWwindow* window, mouse_pos mouse);

    void render(GLFWwindow* window);

private:
    bool first_call;
    std::string state;

    int size;

    Menu menu_obj;
    GUI_Util gui_obj;

    Shader menu_shader;
    Shader* current_shader; 
};

#endif