#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>

class Display
{
public:
    Display(std::string file_path);

private:
    std::string texture_path;
    std::vector<float> vertices;
    std::vector<int> indices;
};

#endif