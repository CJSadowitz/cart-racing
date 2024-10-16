#include "BVH.h"
#include <limits>
#include <utility>
#include <iostream>

BVH::BVH(std::vector<tri>& triangles)
{
    std::cout << "BVH constructor: " << triangles.size() <<std::endl;
    build_bvh(triangles, 0, 0, triangles.size(), 10);

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
    bvh_node new_node;
    glm::vec3 aabb_min = glm::vec3(0);
    glm::vec3 aabb_max = glm::vec3(0);

    for (int i = start; i < end; i++)
    {
        // Print all triangles to visually verify this
        // std::cout << "X: " << triangles[i].vertex0.pos.x << " Y: "<< triangles[i].vertex0.pos.y << " Z: " << triangles[i].vertex0.pos.z << std::endl;
        // std::cout << "X: " << triangles[i].vertex1.pos.x << " Y: "<< triangles[i].vertex1.pos.y << " Z: " << triangles[i].vertex1.pos.z << std::endl;
        // std::cout << "X: " << triangles[i].vertex2.pos.x << " Y: "<< triangles[i].vertex2.pos.y << " Z: " << triangles[i].vertex2.pos.z << std::endl;
        // std::cout << "----------------------------------------------------" << std::endl;
        // Compute centroid
        triangles[i].centroid.pos = (triangles[i].vertex0.pos + triangles[i].vertex1.pos + triangles[i].vertex2.pos) / 3.0f;
    }
    aabb_min = get_aabb_min(triangles, start, end);
    aabb_max = get_aabb_max(triangles, start, end);

    std::cout << "Node " << node_index << " AABB Min: " << aabb_min.x << ", " << aabb_min.y << ", " << aabb_min.z << std::endl;
    std::cout << "Node " << node_index << " AABB Max: " << aabb_max.x << ", " << aabb_max.y << ", " << aabb_max.z << std::endl;
    std::cout << "Start: " << start << " End: " << end << std::endl;

    new_node.aabb_min.pos = aabb_min;
    new_node.aabb_max.pos = aabb_max;

    // find the axis to split about
    glm::vec3 extent = new_node.aabb_max.pos - new_node.aabb_min.pos;
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent[axis]) axis = 2;
    float splitPos = new_node.aabb_min.pos[axis] + extent[axis] * 0.5f;

    new_node.first_prim = start;
    new_node.triangle_count = end - start;
    int i = new_node.first_prim;
    int j = i + new_node.triangle_count - 1;
    while (i <= j)
    {
        if (triangles[i].centroid.pos[axis] < splitPos)
        {
            i++;
        }
        else
        {
            std::swap(triangles[i], triangles[j--]);
        }
    }
    this->node_list[node_index] = new_node;
    // Base case
    if (count <= 0 || end - start <= 3)
    {
        return;
    }
    uint mid = (end - start) / 2 + start;

    // add node to vector and generate children
    
    build_bvh(triangles, node_index * 2 + 1, start, mid, count - 1);
    build_bvh(triangles, node_index * 2 + 2, mid, end, count - 1);
}

// return the value closest to infinity
glm::vec3 BVH::get_aabb_max(std::vector<tri>& triangles, uint start, uint end)
{
    const float large_value = 1e4f;
    glm::vec3 aabb_max = glm::vec3(triangles[0].vertex0.pos);
    float min_dist = distance(aabb_max, large_value);

    for (uint i = start; i < end; ++i) {
        for (const auto& vertex : {triangles[i].vertex0.pos, triangles[i].vertex1.pos, triangles[i].vertex2.pos}) {
            float dist = distance(vertex, large_value);
            // std::cout << dist << ' ' << min_dist << std::endl;
            if (dist < min_dist) {
                min_dist = dist;
                aabb_max = vertex;
            }
            if (dist == min_dist) // supposed to be as high up as possible
            {
                if (aabb_max.y < vertex.y) aabb_max = vertex;
                if (aabb_max.z < vertex.z) aabb_max = vertex;
            }
        }
    }
    return aabb_max;
}

// return the value closest to negative infinity
glm::vec3 BVH::get_aabb_min(std::vector<tri>& triangles, uint start, uint end)
{
    const float large_value = 1e4f;
    glm::vec3 aabb_min = glm::vec3(triangles[0].vertex0.pos);
    float min_dist = distance(aabb_min, -large_value);

    for (uint i = start; i < end; ++i) {
        for (const auto& vertex : {triangles[i].vertex0.pos, triangles[i].vertex1.pos, triangles[i].vertex2.pos}) {
            float dist = distance(vertex, large_value);
            // std::cout << dist << ' ' << min_dist << std::endl;
            if (dist > min_dist) {
                min_dist = dist;
                aabb_min = vertex;
            }
            if (dist == min_dist) // supposed to be as far down as possible...
            {
                if (aabb_min.y > vertex.y) aabb_min = vertex;
                if (aabb_min.z > vertex.z) aabb_min = vertex;
            }
        }
    }
    return aabb_min;
}

float BVH::distance(glm::vec3 point1, float point2)
{
    glm::vec3 diff = point1 - glm::vec3(point2, point2, point2);
    return glm::dot(diff, diff);
}

bool BVH::intersect(const BVH& other_tree)
{
    /* Compare the current node to see if the aabbs intersect, then recursively check for children */
    return false;
}

void BVH::render_aabb(Camera& camera)
{
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    // std::cout << "Rendering: " << this->node_list.size() << " nodes" << std::endl;
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

void BVH::print_node_values()
{
    std::cout << "BVH NODE VERTICES" << std::endl;
    for (int i = 0; i < this->node_list.size(); i++)
    {
        std::cout << "Left first: " << this->node_list[i].first_prim << std::endl;
        std::cout << "Triangle Count: " << this->node_list[i].triangle_count << std::endl;
        std::cout << i << ": ";
        std::cout << this->node_list[i].aabb_min.pos.x << ' ';
        std::cout << this->node_list[i].aabb_min.pos.y << ' ';
        std::cout << this->node_list[i].aabb_min.pos.z << ", ";

        std::cout << this->node_list[i].aabb_max.pos.x << ' ';
        std::cout << this->node_list[i].aabb_max.pos.y << ' ';
        std::cout << this->node_list[i].aabb_max.pos.z << '\n';
    }
    std::cout << std::endl;
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