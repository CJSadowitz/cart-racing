#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "Model.h"
#include "Camera.h"

class Scene
{
public:
    Scene(std::string file_path);

    void render(Camera camera);
    std::vector<Model> get_models();

private:
    std::vector<Model> models;

};

#endif