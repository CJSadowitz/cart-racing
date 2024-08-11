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

void State_Machine::run_state(GLFWwindow* window, mouse_pos mouse)
{
    if (this->state == "main_menu")
    {
        main_menu_state(window, mouse);
    }
    else if (this->state == "settings_menu")
    {
        settings_menu_state(window, mouse);
    }
    else if (this->state == "game_state")
    {
        game_state(window, mouse);
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

void State_Machine::main_menu_state(GLFWwindow* window, mouse_pos mouse)
{
    if (this->first_call != false)
    {
        this->first_call = false;
        this->menu_obj = Menu();

        // Theres got to be a way to condense this mess
        this->gui_obj = GUI_Util();
        float b_left[] = {-0.875, -0.875};
        float t_right[] = {-0.6, -0.6};
        float color[] = {1.0, 0.0, 0.0};
        rect_positions rect = this->gui_obj.set_rect_positions(b_left, t_right, color);
        
        // This value should not be hardcoded. It is dependent on the size of the vertex array
        this->size = 144;
        this->menu_obj.generate(rect.positions, this->size);
        
        delete rect.positions;
    }

    // Render
    // ---------------------------------------------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    this->current_shader->use();
    this->menu_obj.bind();
    // The six needs to be updated. Should not be hardcoded.
    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->menu_obj.unbind();
    glfwSwapBuffers(window);
    // ---------------------------------------------
    int index = this->gui_obj.is_pressed(window, mouse);
    if (index != -1)
    {
        std::cout << index << std::endl;
    }
}

void State_Machine::settings_menu_state(GLFWwindow* window, mouse_pos mouse)
{
    
}

void State_Machine::game_state(GLFWwindow* window, mouse_pos mouse)
{
    
}
