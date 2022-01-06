//
// Created by User on 12/29/2021.
//

#include <clioptions.h>
#include <backnrenderer.h>
#include <fntrenderer.h>


namespace Application
{
    Text::Renderer* pTxtRndr{0x0};
    GLFWwindow* pWindow{0x0};

    void processInput()
    {
        if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(pWindow, true);
    }
    void Init(int argc, char**argv)
    {

        auto options = FONTTEST_OPTIONS::ParseOptions(argc, argv);


        Backend::Init();
        pWindow = Backend::Window::Create(glm::ivec2(1200,800), "Text Rendering Sample");
        if (!pWindow){
            Backend::Finish();
            return;
        }

        Backend::Context::Set(pWindow);

        if (Renderer::Init())
        {
            std::cout << "There was a problem, initializing open GL" << std::endl;
            Backend::Window::Destroy(pWindow);
            Backend::Finish();
            return;
        }

        //Do Some Parameter validation here.
        pTxtRndr = new Text::Renderer(options.vtxShdrPath, options.frgShdrPath, options.fontPath);

    }

    void Loop()
    {
        if (!pWindow){
            std::cout << "There was a problem, no window present" << std::endl;
        }
        while(!glfwWindowShouldClose(pWindow))
        {
            processInput();
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            pTxtRndr->Draw("This is sample Text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
            pTxtRndr->Draw("(C) LizzardDev.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3f, 0.7f, 0.9f));
            glfwSwapBuffers(pWindow);
            glfwPollEvents();
        }
        glfwTerminate();
    }

}


int main(int argc, char** argv)
{

    Application::Init(argc, argv);
    Application::Loop();
    return 0;

}