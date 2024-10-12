#include "Scene.h"

// ex: assets/scenes/title_scene
Scene::Scene(const std::string& file_path)
{
    for (const auto& entry: std::filesystem::directory_iterator(file_path + "/models"))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            // create a model for each model directory in the scene folder
            auto file_path = entry.path();
            this->models.push_back(Model(file_path.string()));
        }
    }
    for (const auto& entry: std::filesystem::directory_iterator(file_path + "/lights"))
    {
        if (std::filesystem::is_directory(entry.status()))
        {
            // create a model for each model directory in the scene folder
            auto file_path = entry.path();
            this->lights.push_back(Light(file_path.string()));
        }
    }
    for (int i = 0; i < this->lights.size(); i++)
    {
        this->lights[i].generate_buffers();
    }
}

void Scene::render(Camera& camera)
{
    for (int i = 0; i < this->models.size(); i++)
    {
        this->models[i].model_shaders[0].use();
        if (this->lights.size() != 0)
        {
            this->models[i].model_shaders[0].setVec3("lightPos", this->lights[0].get_position(0));
        }
        this->models[i].render(camera);
    }
    for (int i = 0; i < this->lights.size(); i++)
    {
        this->lights[i].render(camera);
    }
    for (int i = 0; i < this->models.size(); i++)
    {
        for (int j = 0; j < this->models[i].BVHs.size(); j++)
        {
            this->models[i].BVHs[j].render_aabb(camera);
            this->models[i].BVHs[j].print_node_values();
        }
    }
}

std::vector<Model> Scene::get_models() // collisions ;D
{
    return this->models;
}
