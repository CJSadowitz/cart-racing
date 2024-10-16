#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "include/State_Machine.h"
#include "Input.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Pain", primaryMonitor, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowAspectRatio(window, 4, 3);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    State_Machine my_machine;
    mouse_pos mouse;
    glfwSetWindowUserPointer(window, &mouse);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST); // culling
    // glEnable(GL_BLEND); // enable alpha this is expensive
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        my_machine.run_state(window, mouse);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouse_pos* cursorPos = static_cast<mouse_pos*>(glfwGetWindowUserPointer(window));
    cursorPos->x_pos = xpos;
    cursorPos->y_pos = ypos;
}
