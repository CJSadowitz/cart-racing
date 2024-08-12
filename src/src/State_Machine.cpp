#include "State_Machine.h"
#include <iostream>
#include <vector>

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
        this->gui_obj = GUI_Util();

        // Theres got to be a way to condense this mess
        float b_left1[] = {-0.875, -0.875};
        float t_right1[] = {-0.6, -0.6};
        float color1[] = {1.0, 0.0, 0.0};
        rect_positions rect1 = this->gui_obj.set_rect_positions(b_left1, t_right1, color1);
        std::vector<float> all_pos(rect1.positions, rect1.positions + 36);
        
        float b_left2[] = {-0.875, -0.575};
        float t_right2[] = {-0.6, -0.2};
        float color2[] = {0.0, 1.0, 0.0};
        rect_positions rect2 = this->gui_obj.set_rect_positions(b_left2, t_right2, color2);
        std::vector<float> button_two(rect2.positions, rect2.positions + 36);

        all_pos.insert(all_pos.end(), button_two.begin(), button_two.end());
        s
        // Each button is 144 bytes
        this->size = all_pos.size() * sizeof(float);
        this->menu_obj.generate(all_pos.data(), this->size);
        
        delete rect1.positions;
        delete rect2.positions;
    }
    
    render(window);
    
    int index = this->gui_obj.is_hover(window, mouse);
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

void State_Machine::render(GLFWwindow* window)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    this->current_shader->use();
    this->menu_obj.bind();
    // color rect render: requires / 24 because each button is 144 bytes
    glDrawArrays(GL_TRIANGLES, 0, this->size / 24);
    this->menu_obj.unbind();
    glfwSwapBuffers(window);
}
