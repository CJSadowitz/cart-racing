#include "GUI.h"
#include <filesystem> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
            auto button_path = entry.path();
            this->buttons.push_back(Button(button_path.string() + "/meshes"));
            this->button_shaders.push_back(Shader((button_path.string() + "/shaders/vertex.vs").c_str(), (button_path.string() + "/shaders/fragment.fs").c_str()));
            file_count++;
        }
    }
    for (const auto& entry: std::filesystem::directory_iterator(file_path + "/display"))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            // create a display for each display directory in the displays folder
            auto display_path = entry.path();
            this->displays.push_back(Display(display_path.string()));
            this->display_shaders.push_back(Shader((display_path.string() + "/shaders/vertex.vs").c_str(), (display_path.string() + "/shaders/fragment.fs").c_str()));
            file_count++;
        }
    }

    this->B_VAOs.resize(file_count);
    this->B_VBOs.resize(file_count);
    this->B_EBOs.resize(file_count);
    glGenVertexArrays(file_count, this->B_VAOs.data());
    glGenBuffers(file_count, this->B_VBOs.data());
    glGenBuffers(file_count, this->B_EBOs.data());

    this->D_VAOs.resize(file_count);
    this->D_VBOs.resize(file_count);
    this->D_EBOs.resize(file_count);
    glGenVertexArrays(file_count, this->D_VAOs.data());
    glGenBuffers(file_count, this->D_VBOs.data());
    glGenBuffers(file_count, this->D_EBOs.data());

    generate_buttons();
    generate_displays();

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.125, 0.125, 0.125));
    // transform = glm::translate(transform, glm::vec3(0.0, -8.0, 0.0));
    this->transform = transform;

    for (int i = 0; i < this->buttons.size(); i++)
    {
        this->buttons[i].set_button_rect_positions(transform);
    }

}

void Gui::generate_buttons()
{
    for (int i = 0; i < this->buttons.size(); i++)
    {
        button_bind(i);
        glBufferData(GL_ARRAY_BUFFER, this->buttons[i].get_vertices_size() * 4, this->buttons[i].get_vertices_vector().data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->B_EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->buttons[i].get_indices_size() * 4, this->buttons[i].get_indices_vector().data(), GL_STATIC_DRAW);

        // vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // textures
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // set box collider positions for menu
    }
}

void Gui::generate_displays()
{
    for (int i = 0; i < this->displays.size(); i++)
    {
        display_bind(i);
        glBufferData(GL_ARRAY_BUFFER, this->displays[i].get_vertices_size() * 4, this->displays[i].get_vertices_vector().data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->D_EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->displays[i].get_indices_size() * 4, this->displays[i].get_indices_vector().data(), GL_STATIC_DRAW);

        // vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // textures
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}

void Gui::button_bind(int index)
{
    glBindVertexArray(this->B_VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, this->B_VBOs[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->B_EBOs[index]);
}

void Gui::display_bind(int index)
{
    glBindVertexArray(this->D_VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, this->D_VBOs[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->D_EBOs[index]);
}

void Gui::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Gui::render()
{   
    for (int i = 0; i < this->buttons.size(); i++)
    {
        button_bind(i);
        this->button_shaders[i].use();
        this->button_shaders[i].setMat4("transform", this->transform);
        glDrawElements(GL_TRIANGLES, this->buttons[i].get_indices_size(), GL_UNSIGNED_INT, 0);
        unbind();
    }
    for (int i = 0; i < this->displays.size(); i++)
    {
        display_bind(i);
        this->display_shaders[i].use();
        this->display_shaders[i].setMat4("transform", this->transform);

        glDrawElements(GL_TRIANGLES, this->displays[i].get_indices_size(), GL_UNSIGNED_INT, 0);
        unbind();
    }
}

std::vector<Button> Gui::get_buttons()
{
    return this->buttons;
}

std::vector<Display> Gui::get_displays()
{
    return this->displays;
}
