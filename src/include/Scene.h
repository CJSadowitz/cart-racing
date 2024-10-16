#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "BVH.h"

class Scene
{
public:
    Scene(const std::string& file_path);

    void render(Camera& camera);
    std::vector<Model> get_models();

private:
    std::vector<Model> models;
    std::vector<Light> lights;

};

#endif