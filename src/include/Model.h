#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"

class Model
{
    void bind(int index);
    void unbind();

    void add_mesh(Mesh mesh);

    void rotate_mesh(int index);
    void translate_mesh(int index);
    void scale_mesh(int index);

private:

    void generate_buffers(Mesh mesh);

    std::vector<Mesh> meshes;
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vecotr<GLuint> EBOs;
};

#endif