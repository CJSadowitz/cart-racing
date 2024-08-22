#include "Scene.h"

// ex: assets/scenes/title_scene/models
Scene::Scene(std::string file_path)
{
    for (const auto& entry: std::filesystem::directory_iterator(file_path))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            // create a model for each model directory in the scene folder
            auto file_path = entry.path();
            std::cout << file_path.string() << std::endl;
            this->models.push_back(Model(file_path.string()));
        }
    }
    for (int i = 0; i < this->models.size(); i++)
    {
        this->models[i].generate_buffers();
    }
}

void Scene::render()
{
    for (int i = 0; i < this->models.size(); i++)
    {
        this->models[i].render();
    }
}
