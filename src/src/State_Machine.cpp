#include "State_Machine.h"


// instantiate and store each shader upon creation of this object. There must be a better way to do this
State_Machine::State_Machine()
{
    this->state = "main_menu";
    this->first_call = true;
}

void State_Machine::run_state(GLFWwindow* window, mouse_pos mouse)
{
    if (this->state == "main_menu") //consider enum
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
    if (this->first_call != false) // fix initialization (abstract outside of funny loop)
    {
        this->first_call = false;
        this->previous_state = GLFW_RELEASE;
        this->scene_obj.push_back(Scene("assets/scenes/title_scene/models"));
        this->camera_obj.push_back(Camera(glm::vec3(0.0f, 2.0f, 10.0f)));
        this->my_gui_obj.push_back(Gui("assets/hud/title_screen"));
    }
    
    // Renderer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scenes here :D
    this->camera_obj[0].updateCameraPosition(10.0f, this->angle, glm::vec3(0.0f, 0.5f, 0.0f));
    this->scene_obj[0].render(this->camera_obj[0]);
    this->angle += 1;
    this->my_gui_obj[0].render(window, mouse);

    glfwSwapBuffers(window);
    
    // Menu Changer
    int current_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int index = this->my_gui_obj[0].get_button_pressed();
    if (index != -1 && previous_state == GLFW_PRESS && current_state == GLFW_RELEASE)
    {
        if (index == 0)
        {
            size.clear();
            set_state("game_state");
        }
        else if (index == 1)
        {
            size.clear();
            set_state("settings_menu");
        }
    }
    this->previous_state = current_state;
}

void State_Machine::settings_menu_state(GLFWwindow* window, mouse_pos mouse)
{
    if (this->first_call != false)
    {
        this->first_call = false;
        this->previous_state = GLFW_RELEASE;
    }
    // Renderer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // scene and gui obj here

    glfwSwapBuffers(window);
    
    // // Menu Changer
    // int current_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    // int index = this->gui_obj.is_hover(window, mouse);
    // if (index != -1 && previous_state == GLFW_PRESS && current_state == GLFW_RELEASE)
    // {
    //     if (index == 0)
    //     {
    //         std::cout << "Top Button" << std::endl;
    //     }
    //     else if (index == 1)
    //     {
    //         std::cout << "Bottom Button" << std::endl;
    //     }
    // }
    // this->previous_state = current_state;
}

void State_Machine::game_state(GLFWwindow* window, mouse_pos mouse)
{
    std::cout << "We are in game state" << std::endl;
}
