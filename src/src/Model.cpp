#include "Model.h"
#include "Camera.h"
#include <stb_image.h>

// ex: assets/scenes/title_scene/models/model_name
Model::Model(const std::string& mesh_path)
{
    int file_count = 0;
    int texture_count = 0;
    // for every obj file create a mesh object and add it to a vector
    for (const auto& entry: std::filesystem::directory_iterator(mesh_path + "/meshes"))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            file_count++;
            auto file_path = entry.path();
            this->meshes.push_back(Mesh(file_path.string()));
        }
    }
    try
    {
        bool textures_found = false;
        for (const auto& entry: std::filesystem::directory_iterator(mesh_path + "/textures"))
        {
            if (std::filesystem::is_regular_file(entry.status()))
            {
                textures_found = true;
                texture_count++;
                auto file_path = entry.path();
                this->texture_paths.resize(texture_count);
                this->texture_paths.push_back(file_path.string());
            }
        }
        if (!textures_found)
        {
            this->texture_paths.push_back("");
            throw std::runtime_error("No textures found");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Model Error: " << e.what() << std::endl;
    }
    this->VAOs.resize(file_count); // THIS IS REQUIRED DO NOT DELETE IT RE-RE
    this->VBOs.resize(file_count);
    this->EBOs.resize(file_count);
    glGenVertexArrays(file_count, this->VAOs.data());
    glGenBuffers(file_count, this->VBOs.data());
    glGenBuffers(file_count, this->EBOs.data());
    this->model_shaders.push_back(Shader((mesh_path + "/shaders/vertex.vs").c_str(), (mesh_path + "/shaders/fragment.fs").c_str()));

    this->file_count = file_count;

    generate_buffers();
    // generate BVH for each mesh apart of this model
    std::cout << "Before BVH" << std::endl;
    for (int i = 0; i < this->meshes.size(); i++)
    {
        this->BVHs.push_back(BVH(this->meshes[i].triangles));
    }
    std::cout << "After All BVH creation" << std::endl;
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
            data = stbi_load(std::filesystem::path(this->texture_paths[i]).c_str(), &width, &height, &nrChannels, 0);
            if (!data)
            {
                throw std::runtime_error("Failed to load texture");
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Model Error: " << e.what() << std::endl;
            data = stbi_load(std::filesystem::path("assets/util/textures/missing_texture.png").c_str(), &width, &height, &nrChannels, 0);
            if (!data)
            {
                std::cout << "Model Error: Failed to open missing_texture.png" << std::endl;
            }
        }
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        this->textures.push_back(texture1);
        unbind();
    }
}

void Model::render(Camera& camera)
{
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;    

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textures[0]); // still works because there is something here just with no info

    for (int i = 0; i < this->meshes.size(); i++)
    {
        this->model_shaders[i].use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        this->model_shaders[i].setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        this->model_shaders[i].setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        this->model_shaders[i].setMat4("model", model);

        this->model_shaders[i].setVec3("viewPos", camera.Position);
        this->model_shaders[i].setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        bind(i);
        glDrawElements(GL_TRIANGLES, this->meshes[i].get_indices_size(), GL_UNSIGNED_INT, 0);
        unbind();
    }
}
