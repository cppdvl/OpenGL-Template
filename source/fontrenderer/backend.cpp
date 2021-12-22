//
// Created by User on 12/22/2021.
//

#include <vector>
#include <utility>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Backend {
    static void Init(GLFWwindow**ppWindow, glm::ivec2 windowSize){
        const char* glsl_version = "#version 330";
        glfwInit();

        std::vector<std::pair<int, int>> backend_cofig{
            {GLFW_CONTEXT_VERSION_MAJOR,3},
            {GLFW_CONTEXT_VERSION_MINOR,3},
            {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE}
        };

        std::for_each(backend_cofig.begin(), backend_cofig.end(), [](const auto& vp)
        {
            glfwWindowHint(vp.first, vp.second);
        });

        

    }
}