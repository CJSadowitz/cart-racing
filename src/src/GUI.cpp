#include "GUI.h"
#include <filesystem> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

// ex: assets/hud/title_screen
Gui::Gui(const std::string& file_path)
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
            this->button_file_path.push_back(button_path.string());
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
            this->display_file_path.push_back(display_path.string());
            file_count++;
        }
    }
    bool textures_found = false;
    int texture_count = 0;
        // ex: assets/hud/title_screen/buttons/play add /textures look through for regular file
    for (int i = 0; i < this->button_file_path.size(); i++)
    {
        try
        {
            for (const auto& entry: std::filesystem::directory_iterator(this->button_file_path[i] + "/textures"))
            {
                std::cout << this->button_file_path[i] + "/textures" << std::endl;
                if (std::filesystem::is_regular_file(entry.status()))
                {
                    textures_found = true;
                    texture_count++;
                    auto file_path = entry.path();
                    this->button_texture_paths.resize(texture_count);
                    this->button_texture_paths.push_back(file_path.string());
                }
            }
            if (!textures_found)
            {
                this->button_texture_paths.push_back("");
                throw std::runtime_error("No textures found");
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "GUI Error: " << e.what() << std::endl;
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
    transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
    // transform = glm::translate(transform, glm::vec3(0.0, 0.0, 0.0));
    this->transform = transform;

    // Mouse collider box for menu selection
    for (int i = 0; i < this->buttons.size(); i++)
    {
        this->buttons[i].set_button_rect_positions(transform);
    }
    this->button_index = -1;
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

        // texture generation:
        unsigned int texture1; // Assume one texture per model:
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); 
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load image, create texture and generate mipmaps

        int width, height, nrChannels;
        unsigned char* data;
        try
        {
            if (this->button_texture_paths.size() == 0)
            {
                throw std::runtime_error("Empty texture path");
            }
            else
            {
                data = stbi_load(std::filesystem::path(this->button_texture_paths[i]).c_str(), &width, &height, &nrChannels, 0);
            }

            if (!data)
            {
                throw std::runtime_error("Failed to load texture");
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "GUI Error: " << e.what() << std::endl;
            data = stbi_load(std::filesystem::path("assets/util/textures/missing_texture.png").c_str(), &width, &height, &nrChannels, 0);
            if (!data)
            {
                std::cout << "GUI Error: Failed to open missing_texture.png" << std::endl;
            }
        }
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        this->button_textures.push_back(texture1);
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

        // texture generation:
        unsigned int texture1; // Assume one texture per model:
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); 
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load image, create texture and generate mipmaps

        int width, height, nrChannels;
        unsigned char* data;
        try
        {
            if (this->display_texture_paths.size() == 0)
            {
                throw std::runtime_error("Empty texture path");
            }
            else
            {
                data = stbi_load(std::filesystem::path(this->display_texture_paths[i]).c_str(), &width, &height, &nrChannels, 0);
                std::cout << "Here" << std::endl;
            }

            if (!data)
            {
                throw std::runtime_error("Failed to load texture");
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "GUI Error: " << e.what() << std::endl;
            data = stbi_load(std::filesystem::path("assets/util/textures/missing_texture.png").c_str(), &width, &height, &nrChannels, 0);
            if (!data)
            {
                std::cout << "GUI Error: Failed to open missing_texture.png" << std::endl;
            }
        }
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        this->button_textures.push_back(texture1);
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

void Gui::render(GLFWwindow* window, mouse_pos mouse)
{   
    for (int i = 0; i < this->buttons.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->button_textures[0]); // still works because there is something here just with no info

        if (this->buttons[i].clicked(window, mouse))
        {
            this->button_index = i;
        }

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

const std::vector<Button>& Gui::get_buttons()
{
    return this->buttons;
}

const std::vector<Display>& Gui::get_displays()
{
    return this->displays;
}

int Gui::get_button_pressed()
{
    return this->button_index;
}
