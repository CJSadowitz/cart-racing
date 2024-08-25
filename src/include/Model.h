#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <filesystem>   
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

class Model
{
public:
    Model(std::string mesh_path);

    void render(Camera camera);

    void generate_buffers();

private:
    void bind(int index);
    void unbind();

    std::vector<Shader> model_shaders;
    std::vector<std::string> texture_paths;

    std::vector<Mesh> meshes;
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<GLuint> EBOs;
    std::vector<GLuint> textures;

    size_t file_count;
};

#endif