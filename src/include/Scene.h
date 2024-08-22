#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "Model.h"

class Scene
{
public:
    Scene(std::string file_path);

    void render();

private:
    std::vector<Model> models;

};

#endif