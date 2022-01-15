//
// Created by User on 1/4/2022.
//

#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Renderer {
    /* Init your rendering context (YOU MUST FIRST INIT YOU BANCKEND */
    int Init();
}
namespace Backend {

    /* Init your backend */
    void Init();

    /* Finish The Backend */
    void Finish();
}
namespace Backend::Window{

    /* Create a Window */
    GLFWwindow* Create(const unsigned int, const unsigned int, const std::string);//w,h,title
    GLFWwindow* Create(glm::ivec2 size, const std::string); //size title

    /* Update a window and poll events */
    void SwapAndPoll(GLFWwindow*);

    /* Destroy a window. */
    void Destroy(GLFWwindow*);

}

namespace Backend::Context {
    //Set the actual window as the current context
    //Set the actual window as the current context
    void Set(GLFWwindow*);
}


namespace Backend::Callbacks
{
    void framebuffer_size(GLFWwindow*,int,int);
}

