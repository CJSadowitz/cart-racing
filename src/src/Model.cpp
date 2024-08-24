#include "Model.h"
#include "Camera.h"

// ex: assets/scenes/title_scene/models/model_name
Model::Model(std::string mesh_path)
{
    this->angle = 0;
    int file_count = 0;
    for (const auto& entry: std::filesystem::directory_iterator(mesh_path + "/meshes"))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            file_count++;
            auto file_path = entry.path();
            this->meshes.push_back(Mesh(file_path.string()));
        }
    }
    this->VAOs.resize(file_count);
    this->VBOs.resize(file_count);
    this->EBOs.resize(file_count);
    glGenVertexArrays(file_count, this->VAOs.data());
    glGenBuffers(file_count, this->VBOs.data());
    glGenBuffers(file_count, this->EBOs.data());
    this->model_shaders.push_back(Shader((mesh_path + "/shaders" + "/vertex.vs").c_str(), (mesh_path + "/shaders" + "/fragment.fs").c_str()));

    this->file_count = file_count;
}

void Model::bind(int index)
{
    glBindVertexArray(this->VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOs[index]);
}

void Model::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::generate_buffers()
{
    for (int i = 0; i < this->file_count; i++)
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

void Model::render()
{

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;    
    
    Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

    for (int i = 0; i < this->meshes.size(); i++)
    {
        this->model_shaders[i].use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        this->model_shaders[i].setMat4("projection", projection);

        this->angle += 0.1f; // Adjust the speed of rotation as needed
        if (this->angle >= 360.0f) this->angle = 0.0f;

        // Update the camera position
        camera.updateCameraPosition(30, this->angle, glm::vec3(0.0f, 5.0f, 0.0f));

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        this->model_shaders[i].setMat4("view", view);

        bind(i);
        glDrawElements(GL_TRIANGLES, this->meshes[i].get_indices_size(), GL_UNSIGNED_INT, 0);
        unbind();
    }
}
