#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

class Display
{
public:
    Display(const std::string& file_path);

    const std::vector<float>& get_vertices_vector();
    const std::vector<int>& get_indices_vector();

    int get_indices_size();
    float get_vertices_size();

private:
    std::string texture_path;
    std::vector<float> vertices;
    std::vector<int> indices;
};

#endif