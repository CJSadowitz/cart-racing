#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <filesystem>   
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
    Model(std::string mesh_path);

    void render();

    void generate_buffers();

    void rotate_mesh(int index);
    void translate_mesh(int index);
    void scale_mesh(int index);

private:
    void bind(int index);
    void unbind();

    float angle;

    std::vector<Shader> model_shaders;

    std::vector<Mesh> meshes;
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<GLuint> EBOs;

    size_t file_count;
};

#endif