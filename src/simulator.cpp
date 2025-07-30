// ImGUI
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// GLAD
#include <glad/glad.h>

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
#include "renderer.h"
#include "particle.h"

#include "simulator.h"

Simulator::Simulator(unsigned int width, unsigned int height)
    : width(width), keys(), height(height), state(SIMULATOR_STATE_IDLE)
{
    physicsEngine = PhysicsEngine();
}

Simulator::~Simulator()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow *Simulator::Init()
{
    if (!(this->window = initGLFW()))
    {
        std::cerr << "ERROR::SIMULATOR::INIT: Failed to set up GLFW window." << std::endl;
        return nullptr;
    }

    initImGui(window);

    loadResources();
    return window;
}

void Simulator::ProcessInput(float delta)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        physicsEngine.addParticle(Particle(0, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        std::cout << "Particle added at (100, 100) with zero velocity." << std::endl;
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        physicsEngine.update(1.0f);
        std::cout << "Physics engine updated." << std::endl;
}

void Simulator::Update(float delta)
{
}

void Simulator::Render()
{
    Shader circleShader = ResourceManager::GetShader("circleShader");
    // Calculate orthographic projection matrix
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);
    circleShader.SetMat4("projection", projection);
    Renderer circleRenderer(circleShader);

    while (!glfwWindowShouldClose(window))
    {
        // Input
        ProcessInput(0.0f);

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.42f, 0.0f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (physicsEngine.getParticles().empty())
        {
            std::cout << "No particles to render." << std::endl;
        }
        else {
            circleRenderer.getShader().SetVec2f("u_resolution", display_w, display_h);
            circleRenderer.render(physicsEngine.getParticles().front().position, glm::vec2(0.25f, 0.25f), 0.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        // ImGui rendering
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Call & Swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow *Simulator::initGLFW()
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
    glfwSwapInterval(1);
    

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

int Simulator::initImGui(GLFWwindow *window)
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

void Simulator::loadResources()
{
    ResourceManager::LoadShader("shaders/circle.vert", "shaders/circle.frag", nullptr, "circleShader");
}