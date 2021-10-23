#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


struct GUI {

    static void Init(GLFWwindow* window, const char* glsl_version)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
    static void StartInThisFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    static void FinishGui_InThisFrame()
    {
        ImGui::Render();
    }

    static void DrawGui_BeginWidget(const char* title)
    {
        ImGui::Begin(title);
    }
    
    static void DrawGui_TextWidget(const char* message)
    {
        ImGui::Text(message);
    }

    static void DrawGui_EndWidget()
    {
        ImGui::End();
    }
    static void Draw()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


};



int main()
{
    const char* glsl_version = "#version 330";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Construct the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Template", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    GUI::Init(window, glsl_version);

    // Handle view port dimensions
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    // This is the render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ProcessInput(window);


        GUI::StartInThisFrame();
        {
            GUI::DrawGui_BeginWidget("Hello World Widget");
            GUI::DrawGui_TextWidget("No Matter How Strong, Im Gonna Take You Down with one little stone");
            GUI::DrawGui_EndWidget();
        }
        GUI::FinishGui_InThisFrame();

        // Druids are the best
        glClearColor(1.00f, 0.49f, 0.04f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        GUI::Draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}