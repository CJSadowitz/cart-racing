#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

struct mouse_pos {
    float x_pos;
    float y_pos;
};

class Input {
public:
    void update_mouse_pos(GLFWwindow *window, mouse_pos mouse);
    mouse_pos get_mouse_pos();

private:
    mouse_pos mouse;
};

#endif