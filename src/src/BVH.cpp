#include "BVH.h"
#include <limits>
#include <utility>
#include <iostream>

BVH::BVH(std::vector<tri>& triangles)
{
    std::cout << "BVH constructor: " << triangles.size() <<std::endl;
    build_bvh(triangles, 0, 0, triangles.size(), 20);

    // debug rendering code
    this->VAOs.resize(this->node_list.size());
    this->VBOs.resize(this->node_list.size());
    this->EBOs.resize(this->node_list.size());
    glGenVertexArrays(this->node_list.size(), this->VAOs.data());
    glGenBuffers(this->node_list.size(), this->VBOs.data());
    glGenBuffers(this->node_list.size(), this->EBOs.data());

    this->vertices.resize(this->node_list.size());
    this->indices.resize(this->node_list.size());

    for (int i = 0; i < this->node_list.size(); i++)
    {
        generate_vertex_data(i);
        generate_buffers(i);
    }
    this->shaders.push_back(Shader(("assets/util/shaders/bvh_vertex.vs"), ("assets/util/shaders/bvh_fragment.fs")));
}

void BVH::build_bvh(std::vector<tri>& triangles, uint node_index, uint start, uint end, int count)
{
    if (node_index >= this->node_list.size())
    {
        this->node_list.resize(node_index + 1);
    }
    this->node_list.resize(this->node_list.size() + 1);
    bvh_node new_node = this->node_list[node_index];
    // std::cout << "Node_index: " << node_index << "list size: " << this->node_list.size() << std::endl;
    // compute the axis aligned bounding box (AABB)
    glm::vec3 aabb_min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 aabb_max = glm::vec3(std::numeric_limits<float>::lowest());

    for (int i = start; i < end; i++)
    {
        aabb_min = glm::min(aabb_min, glm::min(triangles[i].vertex0.pos, glm::min(triangles[i].vertex1.pos, triangles[i].vertex2.pos)));
        aabb_max = glm::max(aabb_max, glm::max(triangles[i].vertex0.pos, glm::max(triangles[i].vertex1.pos, triangles[i].vertex2.pos)));
        triangles[i].centroid.pos = (triangles[i].vertex0.pos + triangles[i].vertex1.pos + triangles[i].vertex2.pos) / 3.0f;
    }

    // for (tri& t: triangles)
    // {
    //     aabb_min = glm::min(aabb_min, glm::min(t.vertex0.pos, glm::min(t.vertex1.pos, t.vertex2.pos)));
    //     aabb_max = glm::max(aabb_max, glm::max(t.vertex0.pos, glm::max(t.vertex1.pos, t.vertex2.pos)));
    //     t.centroid.pos = (t.vertex0.pos + t.vertex1.pos + t.vertex2.pos) / 3.0f;
    // }
    new_node.aabb_min.pos = aabb_min;
    new_node.aabb_max.pos = aabb_max;
    if (count <= 0 || end - start <= 3)
    {
        std::cout << "Leaf node" << std::endl;
        new_node.triangle_count = end - start;
        return;
    }

    // split and sort triangles
    glm::vec3 extent = new_node.aabb_max.pos - new_node.aabb_min.pos;
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent[axis]) axis = 2;
    float splitPos = new_node.aabb_min.pos[axis] + extent[axis] * 0.5f;

    new_node.left_first = start;
    new_node.triangle_count = end - start;
    int i = new_node.left_first;
    int j = i + new_node.triangle_count - 1;
    while (i <= j)
    {
        if (triangles[i].centroid.pos[axis] < splitPos)
            i++;
        else
            std::swap(triangles[i], triangles[j--]);
    }
    uint mid = (end - start) / 2 + start;
    std::cout << "Count: " << count << " end: " << end << " start: " << start; 
    std::cout << " middle index: " << mid <<std::endl; 

    // add node to vector and generate children
    this->node_list[node_index] = new_node;
    
    build_bvh(triangles, this->node_list.size(), start, mid, count - 1);
    build_bvh(triangles, this->node_list.size() + 1, mid, end, count - 1);
}

bool BVH::intersect(const BVH& other_tree)
{
    return false;
}

void BVH::render_aabb(Camera& camera)
{
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    for (int i = 0; i < this->node_list.size(); i++)
    {
        bind(i);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        this->shaders[i].use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        this->shaders[i].setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        this->shaders[i].setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        this->shaders[i].setMat4("model", model);

        this->shaders[i].setVec3("viewPos", camera.Position);

        glDrawElements(GL_TRIANGLES, this->indices[i].size(), GL_UNSIGNED_INT, 0);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        unbind();
    }
}

void BVH::bind(int index)
{
    glBindVertexArray(this->VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOs[index]);
}

void BVH::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BVH::generate_buffers(int index)
{
    bind(index);
    glBufferData(GL_ARRAY_BUFFER, this->vertices[index].size() * sizeof(float), this->vertices[index].data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOs[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices[index].size() * sizeof(int), this->indices[index].data(), GL_DYNAMIC_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    unbind();
}

void BVH::generate_vertex_data(int index)
{
    this->vertices.push_back(std::vector<float>());
    this->indices.push_back(std::vector<int>());
    
    glm::vec3 aabb_min = this->node_list[index].aabb_min.pos;
    glm::vec3 aabb_max = this->node_list[index].aabb_max.pos;

    // origin
    this->vertices[index].push_back(aabb_min.x);
    this->vertices[index].push_back(aabb_min.y);
    this->vertices[index].push_back(aabb_min.z);

    // right 1
    this->vertices[index].push_back(aabb_max.x);
    this->vertices[index].push_back(aabb_min.y);
    this->vertices[index].push_back(aabb_min.z);

    // up 1, right 1
    this->vertices[index].push_back(aabb_max.x);
    this->vertices[index].push_back(aabb_max.y);
    this->vertices[index].push_back(aabb_min.z);

    // up 1
    this->vertices[index].push_back(aabb_min.x);
    this->vertices[index].push_back(aabb_max.y);
    this->vertices[index].push_back(aabb_min.z);

    // depth 1
    this->vertices[index].push_back(aabb_min.x);
    this->vertices[index].push_back(aabb_min.y);
    this->vertices[index].push_back(aabb_max.z);

    // depth 1, right 1
    this->vertices[index].push_back(aabb_max.x);
    this->vertices[index].push_back(aabb_min.y);
    this->vertices[index].push_back(aabb_max.z);

    // depth 1, up 1, right 1
    this->vertices[index].push_back(aabb_max.x);
    this->vertices[index].push_back(aabb_max.y);
    this->vertices[index].push_back(aabb_max.z);

    // depth 1, up 1
    this->vertices[index].push_back(aabb_min.x);
    this->vertices[index].push_back(aabb_max.y);
    this->vertices[index].push_back(aabb_max.z);

    // front face
    this->indices[index].push_back(0);
    this->indices[index].push_back(1);
    this->indices[index].push_back(2);

    this->indices[index].push_back(2);
    this->indices[index].push_back(3);
    this->indices[index].push_back(0);

    // right face
    this->indices[index].push_back(1);
    this->indices[index].push_back(5);
    this->indices[index].push_back(6);

    this->indices[index].push_back(6);
    this->indices[index].push_back(2);
    this->indices[index].push_back(1);

    // back face
    this->indices[index].push_back(5);
    this->indices[index].push_back(4);
    this->indices[index].push_back(7);

    this->indices[index].push_back(7);
    this->indices[index].push_back(6);
    this->indices[index].push_back(5);

    // left face
    this->indices[index].push_back(4);
    this->indices[index].push_back(0);
    this->indices[index].push_back(3);

    this->indices[index].push_back(3);
    this->indices[index].push_back(7);
    this->indices[index].push_back(4);

    // top face
    this->indices[index].push_back(3);
    this->indices[index].push_back(2);
    this->indices[index].push_back(6);

    this->indices[index].push_back(6);
    this->indices[index].push_back(7);
    this->indices[index].push_back(3);

    // bottom face
    this->indices[index].push_back(4);
    this->indices[index].push_back(5);
    this->indices[index].push_back(1);

    this->indices[index].push_back(1);
    this->indices[index].push_back(0);
    this->indices[index].push_back(4);
}