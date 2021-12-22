#include "objloader.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImFileDialog.h"

const int WindowWidth = 1200;
const int WindowHeight = 800;

namespace RenderSystem {
    static bool sValid {false};
    static std::string sGLSL_VERSION{"#version 330"};
    static void Init(GLFWwindow** ppWindow, glm::ivec2 windowSize)
    {
        const char* glsl_version = "#version 330";
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Construct the window
        *ppWindow = glfwCreateWindow(windowSize.x, windowSize.y, "OpenGL Template", nullptr, nullptr);
        auto pWindow = ppWindow[0];
        if (!pWindow)
        {
            std::cout << "Failed to create the GLFW window\n";
            glfwTerminate();
        }

        glfwMakeContextCurrent(pWindow);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD\n";
            return;
        }
        sValid = true;
    }
    static void Shutdown()
    {
        glfwTerminate();
    }
}

namespace GUI {

    static void Init(GLFWwindow* wnd, const char* glsl_version){

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsLight();

        //Renderer and backend
        ImGui_ImplGlfw_InitForOpenGL(wnd, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

    }
    static void StartInThisFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    static void ProcessInput()
    {

    }

    static void FinishGui_InThisFrame()
    {
        ImGui::Render();
    }

    static void Draw()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}



namespace GUI::OpenFileDialog {

    static void Init()
    {
        ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
            GLuint tex;

            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            return (void *) tex;
        };
        ifd::FileDialog::Instance().DeleteTexture = [](void* tex){
            GLuint texID = (GLuint)((uintptr_t)tex);
            glDeleteTextures(1, &texID);
        };

    }

    static void DrawDialog()
    {
        ImGui::Begin("Control Panel");
        if (ImGui::Button("Open Wavefront Files"))
            ifd::FileDialog::Instance().Open("WaveFrontFileSelector", "Select a .obj and or .mtl", "Wavefront file (*.obj;*.mtl){.obj,.mtl},.*", true);
        ImGui::End();
    }

    static std::vector <std::string> FilesSelected(){
        std::vector<std::string> results{};
        if (ifd::FileDialog::Instance().IsDone("WaveFrontFileSelector")){
            if (ifd::FileDialog::Instance().HasResult()){
                auto paths = ifd::FileDialog::Instance().GetResults();
                for (const auto& path : paths) results.push_back(path.string());
            }
            ifd::FileDialog::Instance().Close();
        }
        return results;
    }

}

namespace Application {

    static glm::ivec2 sWindowsSize(WindowWidth, WindowHeight);

    static void ProcessInput(GLFWwindow* pWindow)
    {
        if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(pWindow, true);
        }
        GUI::ProcessInput();
    }
    static void MainLoop(GLFWwindow* pWindow)
    {
        while (!glfwWindowShouldClose(pWindow))
        {
            glfwPollEvents();
            Application::ProcessInput(pWindow);

            GUI::StartInThisFrame();
            {
                GUI::OpenFileDialog::DrawDialog();
                auto filesSelected = GUI::OpenFileDialog::FilesSelected();
                if (!filesSelected.empty())
                {
                    for (auto& fileSelected : filesSelected)
                    {
                        std::cout << " - " << fileSelected << std::endl;
                    }
                }
            }
            GUI::FinishGui_InThisFrame();

            glClearColor(1.00f, 0.49f, 0.04f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            GUI::Draw();
            glfwSwapBuffers(pWindow);

        }


    }

}


int main(int argc, char**argv)
{

    GLFWwindow* pWindow;


    RenderSystem::Init(&pWindow, Application::sWindowsSize);
    if (!RenderSystem::sValid) return -1;

    GUI::Init(pWindow, RenderSystem::sGLSL_VERSION.c_str());

    Application::MainLoop(pWindow);

    RenderSystem::Shutdown();
    /*BlenderImporter::OBJLoader importer("E:/WorkingDirectory/ModelSamples/sample.obj");
    importer.Visit();*/
    return 0;
}
