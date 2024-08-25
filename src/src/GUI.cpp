#include "GUI.h"
#include <filesystem> 

// ex: assets/hud/title_screen
Gui::Gui(std::string file_path)
{
    // Generate all buttons and displays for the screen
    int file_count = 0;
    for (const auto& entry: std::filesystem::directory_iterator(file_path + "/buttons"))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            // create a button for each button directory in the buttons folder
            auto file_path = entry.path();
            this->buttons.push_back(Button(file_path.string()));
            this->button_shaders.push_back(Shader((file_path.string() + "/shaders/vertex.vs").c_str(), (file_path.string() + "/shaders/fragment.fs").c_str()));
            file_count++;
        }
    }
    for (const auto& entry: std::filesystem::directory_iterator(file_path + "/display"))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            // create a display for each display directory in the displays folder
            auto file_path = entry.path();
            this->displays.push_back(Display(file_path.string()));
            this->display_shaders.push_back(Shader((file_path.string() + "/shaders/vertex.vs").c_str(), (file_path.string() + "/shaders/fragment.fs").c_str()));
            file_count++;
        }
    }

    this->VAOs.resize(file_count);
    this->VBOs.resize(file_count);
    this->EBOs.resize(file_count);
    glGenVertexArrays(file_count, this->VAOs.data());
    glGenBuffers(file_count, this->VBOs.data());
    glGenBuffers(file_count, this->EBOs.data());
}

void Gui::generate_buttons(int index)
{

}

void Gui::generate_displays(int index)
{

}

void Gui::bind(int index)
{

}

void Gui::unbind()
{

}

void Gui::render()
{

}

std::vector<Button> Gui::get_buttons()
{
    return this->buttons;
}

std::vector<Display> Gui::get_displays()
{
    return this->displays;
}
