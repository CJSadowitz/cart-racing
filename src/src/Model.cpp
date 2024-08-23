#include "Model.h"

// ex: assets/scenes/title_scene/models/model_name
Model::Model(std::string mesh_path)
{
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
        glBufferData(GL_ARRAY_BUFFER, this->meshes[i].get_vertices_size(), this->meshes[i].get_vertices_vector().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshes[i].get_indices_size(), this->meshes[i].get_indices_vector().data(), GL_DYNAMIC_DRAW);

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
    this->model_shaders[0].use();
    for (int i = 0; i < this->meshes.size(); i++)
    {
        bind(i);
        glDrawArrays(GL_TRIANGLES, 0, this->meshes[i].get_vertices_vector().size() / 8);
        unbind();
    }
}
