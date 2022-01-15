//
// Created by User on 1/3/2022.
//

#include <string>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Backend
{

    void Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    }
    void Finish()
    {
        glfwTerminate();
    }
}
namespace Backend::Callbacks
{
    void framebuffer_size(GLFWwindow* window, int width, int height)
    {
        glViewport(0,0, width, height);
    }
}
namespace Backend::Window
{

    GLFWwindow* Create(const unsigned int WIDTH = 1200, const unsigned int HEIGHT = 800, std::string windowTitle = "DavidGabriela")
    {
        auto pWindow = glfwCreateWindow(WIDTH, HEIGHT, windowTitle.c_str(), NULL, NULL);
        return pWindow;
    }
    GLFWwindow* Create(glm::ivec2 size, std::string windowTitle = "DavidGabriela")
    {
        return Create(size.x, size.y, windowTitle);
    }
    void Destroy(GLFWwindow*pWindow)
    {
        glfwDestroyWindow(pWindow);
    }

    void SwapAndPoll(GLFWwindow*pWindow)
    {
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}


namespace Backend::Context
{
    void Set(GLFWwindow*pWindow)
    {
        glfwMakeContextCurrent(pWindow);
        glfwSetFramebufferSizeCallback(pWindow, Backend::Callbacks::framebuffer_size);
    }
}

