#include "State_Machine.h"
#include "Menu.h"
#include "GUI_Util.h"
#include <iostream>

State_Machine::State_Machine()
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
    // Create the menu
    // Would this be better to have fully generated and stored rather than dynamically creating it?
    if (this->first_call != false)
    {
        this->menu_obj = Menu();
        this->first_call = false;

        float vertices[] = {
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
        };
        this->menu_obj.generate(vertices, sizeof(vertices));
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->menu_obj.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    this->menu_obj.unbind();
}

void State_Machine::settings_menu_state()
{
    
}

void State_Machine::game_state()
{
    
}
