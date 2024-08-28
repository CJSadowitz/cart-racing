#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Input.h"
#include <iostream>
#include <vector>
#include "Menu_Builder.h" 
#include "Scene.h"
#include "GUI.h"

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

private:
    bool first_call;
    std::string state;
    int previous_state;

    int angle = 0;

    std::vector<Scene> scene_obj;
    std::vector<Camera> camera_obj;
    std::vector<Gui> my_gui_obj;

    std::vector<size_t> size;
};

#endif