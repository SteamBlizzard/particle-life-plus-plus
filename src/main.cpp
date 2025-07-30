// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ Standard Library
#include <iostream>
#include <string>
#include <cmath>

// Project includes
#include "constants.h"
#include "shader.h"
#include "resource_manager.h"




// Function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow *initGLFW();
int compileShaders();
int initImGui(GLFWwindow *window);
void destroy(GLFWwindow *window);
void renderLoop(GLFWwindow *window);
std::string loadShaderSource(const std::string &filepath);

const std::string circleVertPath = "shaders/circle.vert";
const std::string circleFragPath = "shaders/circle.frag";

int main()
{
    GLFWwindow *window = initGLFW();

    if (!window)
    {
        std::cerr << "Failed to set up GLFW window." << std::endl;
        return -1;
    }

    if (initImGui(window) != 0)
    {
        std::cerr << "Failed to initialize ImGui." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    

    renderLoop(window);

    destroy(window);

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow *initGLFW()
{
    if (!glfwInit())
        return nullptr;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Particle Life++", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable vsync
    

    if (!window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return nullptr;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return window;
}

int initImGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void destroy(GLFWwindow *window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void renderLoop(GLFWwindow *window)
{
    Shader circleShader = ResourceManager::LoadShader(circleVertPath.c_str(), circleFragPath.c_str(), nullptr, "circleShader");

    float vertices[] = {
        -0.25f, -0.25f, 0.0f,
        0.25f, -0.25f, 0.0f,
        0.25f, 0.25f, 0.0f,
        -0.25f, 0.25f, 0.0f};

    unsigned int indices[] = {
        0, 1, 2, // first circle
        0, 2, 3  // first circle
    };

    // Define Virtual Buffer Object (VBO)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.42f, 0.0f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        circleShader.Use();
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        circleShader.SetVec4f("input_color", 0.0f, greenValue, 0.0f, 1.0f);
        circleShader.SetVec2f("u_resolution", display_w, display_h);
        circleShader.SetFloat("circle_radius", 0.1f);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ImGui rendering
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::Begin("Particle Life++");
        // ImGui::Text("Welcome to Particle Life++!");
        // ImGui::Text("This is a simple particle simulation application.");
        // ImGui::End();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Call & Swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
