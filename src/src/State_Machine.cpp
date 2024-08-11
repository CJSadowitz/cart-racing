#include "State_Machine.h"
#include <iostream>

// instantiate and store each shader upon creation of this object. There must be a better way to do this
State_Machine::State_Machine()
    : menu_shader("src/shaders/vertex_shader.vs", "src/shaders/fragment_shader.fs"),
      current_shader(&menu_shader)
{
    this->state = "main_menu";
    this->first_call = true;
}

void State_Machine::run_state()
{
    if (this->state == "main_menu")
    {
        main_menu_state();
    }
    else if (this->state == "settings_menu")
    {
        settings_menu_state();
    }
    else if (this->state == "game_state")
    {
        game_state();
    }
}

void State_Machine::set_state(std::string state)
{
    this->first_call = true;
    this->state = state;
}

std::string State_Machine::get_state()
{
    return this->state;
}

// All game states that get ran on loop

void State_Machine::main_menu_state()
{
    if (this->first_call != false)
    {
        this->first_call = false;
        this->menu_obj = Menu();
        GUI_Util myUtil;
        float b_left[] = {-0.875, -0.875};
        float t_right[] = {-0.6, -0.6};
        float color[] = {1.0, 0.0, 0.0};
        rect_positions rect = myUtil.set_rect_positions(b_left, t_right, color);
        
        this->size = 144;
        this->menu_obj.generate(rect.positions, this->size);
        
        delete rect.positions;
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->current_shader->use();

    this->menu_obj.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->menu_obj.unbind();
}

void State_Machine::settings_menu_state()
{
    
}

void State_Machine::game_state()
{
    
}
