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
    else if (this->state == "display_settings")
    {
        game_state(window, mouse);
    }
    else if (this->state == "audio_settings")
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

void State_Machine::clear_objects()
{
    this->scene_obj.clear();
    this->camera_obj.clear();
    this->my_gui_obj.clear();
}

// All game states that get ran on loop
void State_Machine::main_menu_state(GLFWwindow* window, mouse_pos mouse)
{
    if (this->first_call != false) // fix initialization (abstract outside of funny loop)
    {
        this->first_call = false;
        this->previous_state = GLFW_RELEASE;
        this->scene_obj.push_back(Scene("assets/scenes/title_scene"));
        this->camera_obj.push_back(Camera(glm::vec3(0.0f, 4.0f, 0.0f)));
        this->my_gui_obj.push_back(Gui("assets/hud/title_screen"));
    }
    
    // Renderer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scenes here :D
    this->camera_obj[0].updateCameraPosition(10.0f, this->angle, glm::vec3(0.0f, -2.0f, 0.0f));
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
            clear_objects();
            set_state("settings_menu");
        }
        else if (index == 1)
        {
            clear_objects();
            set_state("game_state");
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
        this->scene_obj.push_back(Scene("assets/scenes/settings_scene"));
        this->camera_obj.push_back(Camera(glm::vec3(0.0f, 4.0f, 0.0f)));
        this->my_gui_obj.push_back(Gui("assets/hud/settings_screen"));
    }
    // Renderer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // scene and gui obj here
    this->camera_obj[0].updateCameraPosition(12.0f, this->angle, glm::vec3(0.0f, -2.0f, 0.0f));
    this->scene_obj[0].render(this->camera_obj[0]);
    this->angle += 1;

    this->my_gui_obj[0].render(window, mouse);

    glfwSwapBuffers(window);
    
    int current_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int index = this->my_gui_obj[0].get_button_pressed();
    if (index != -1 && previous_state == GLFW_PRESS && current_state == GLFW_RELEASE)
    {
        if (index == 0)
        {
            // Audio
            std::cout << index << " top buddy" << std::endl;
        }
        else if (index == 1)
        {
            // Back
            clear_objects();
            set_state("main_menu");
        }
        else if (index == 2)
        {
            // Display
            std::cout << index << std::endl;
        }
        else if (index == 3)
        {
            // Controls
            std::cout << index << std::endl;
        }
    }
    this->previous_state = current_state;
}

void State_Machine::game_state(GLFWwindow* window, mouse_pos mouse)
{
    if (this->first_call != false)
    {
        this->first_call = false;
        this->previous_state = GLFW_RELEASE;
        this->scene_obj.push_back(Scene("assets/scenes/test_scene"));
        this->camera_obj.push_back(Camera(glm::vec3(0.0f, 4.0f, 0.0f)));
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // scene and gui obj here
    this->camera_obj[0].updateCameraPosition(19.0f, this->angle, glm::vec3(0.0f, -2.0f, 0.0f));
    this->scene_obj[0].render(this->camera_obj[0]);
    this->angle += 1;

    glfwSwapBuffers(window);
}
