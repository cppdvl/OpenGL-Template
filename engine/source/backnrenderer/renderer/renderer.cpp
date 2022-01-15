//
// Created by User on 1/4/2022.
//

#include "backnrenderer.h"
int Renderer::Init()
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        return -1;
    }
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 0;
}
