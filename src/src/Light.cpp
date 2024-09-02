#include "Light.h"

#include <filesystem>
#include <numeric>

Light::Light(const std::string& file_path)
{
    int file_count = 0;
    for (const auto& entry: std::filesystem::directory_iterator(file_path + "/meshes"))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            file_count++;
            auto file_path = entry.path();
            this->meshes.push_back(Mesh(file_path.string()));
        }
    }
    this->shaders.push_back(Shader((file_path + "/shaders/vertex.vs").c_str(), (file_path + "/shaders/fragment.fs").c_str()));

    this->VAOs.resize(file_count);
    this->VBOs.resize(file_count);
    this->EBOs.resize(file_count);
    glGenVertexArrays(file_count, this->VAOs.data());
    glGenBuffers(file_count, this->VBOs.data());
    glGenBuffers(file_count, this->EBOs.data());
}

void Light::generate_buffers()
{
    for (int i = 0; i < this->meshes.size(); i++)
    {
        bind(i);
        glBufferData(GL_ARRAY_BUFFER, this->meshes[i].get_vertices_size() * 4, this->meshes[i].get_vertices_vector().data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshes[i].get_indices_size() * 4, this->meshes[i].get_indices_vector().data(), GL_DYNAMIC_DRAW);
        // vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // textures
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}

void Light::bind(int index)
{
    glBindVertexArray(this->VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOs[index]);
}

void Light::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Funky Behavior causing annoying problems
glm::vec3 Light::get_position(int index)
{
    // // Find center of light cube:
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    std::vector<float> z_pos;
    std::vector<float> vertices = this->meshes[0].get_vertices_vector();

    for (int i = 0; i < vertices.size(); i += 8)
    {
        x_pos.push_back(vertices[i]);
        y_pos.push_back(vertices[i + 1]);
        z_pos.push_back(vertices[i + 2]);
    }

    float x_center = std::accumulate(x_pos.begin(), x_pos.end(), 0.0f) / x_pos.size();
    float y_center = std::accumulate(y_pos.begin(), y_pos.end(), 0.0f) / y_pos.size();
    float z_center = std::accumulate(z_pos.begin(), z_pos.end(), 0.0f) / z_pos.size();

    return glm::vec3(x_center, y_center, z_center);
}


void Light::render(Camera& camera)
{
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600; 
    for (int i = 0; i < this->meshes.size(); i++)
    {
        this->shaders[i].use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        this->shaders[i].setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        this->shaders[i].setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        this->shaders[i].setMat4("model", model);

        bind(i);
        glDrawElements(GL_TRIANGLES, this->meshes[i].get_indices_size(), GL_UNSIGNED_INT, 0);
        unbind();
    }
}
