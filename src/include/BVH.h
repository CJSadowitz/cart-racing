#ifndef BVH_H
#define BVH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Camera.h"

struct tri
{
    glm::vec3 vertex0;
    glm::vec3 vertex1;
    glm::vec3 vertex2;

    glm::vec3 centroid;
};

struct bvh_node // should be 32 bytes (i dunno about glm::vec3 though)
{
    glm::vec3 aabb_min;
    glm::vec3 aabb_max;

    uint left_first; // if triangle_count is zero it contains the index of the left child node
    uint triangle_count;
};

class BVH
{
public:
    BVH(std::vector<tri>& triangles);
    bool intersect(const BVH& other_tree);

    // debug render code
    void bind(int index);
    void unbind();
    void generate_buffers(int index);

    void generate_vertex_data(int index);
    void render_aabb(Camera& camera);

private:
    void build_bvh(std::vector<tri>& triangles, uint node_index, uint start, uint end, int count);

    std::vector<bvh_node> node_list; // should be an array
    
    // debug code yay
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<GLuint> EBOs;

    std::vector<Shader> shaders;

    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<int>> indices;
};

#endif