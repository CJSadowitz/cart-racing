#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

class Display
{
public:
    Display(std::string file_path);

    std::vector<float> get_vertices_vector();
    std::vector<int> get_indices_vector();

    int get_indices_size();
    float get_vertices_size();

private:
    std::string texture_path;
    std::vector<float> vertices;
    std::vector<int> indices;
};

#endif