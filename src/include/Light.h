#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

class Light
{
public:
    Light(std::string file_path);
    void generate_buffers();
    void render(Camera& camera);

    glm::vec3 get_position(int index);

private:
    void bind(int index);
    void unbind();

    std::vector<Mesh> meshes;
    std::vector<Shader> shaders;
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<GLuint> EBOs;
    std::vector<GLuint> textures;

    size_t file_count;
};

#endif