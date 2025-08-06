// GLAD
#include <glad/glad.h>

// ImGui
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

// C++ Standard Library
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

// Project includes
#include "clock.h"
#include "constants.h"
#include "renderer.h"
#include "resource_manager.h"
#include "shader.h"
#include "configurations.h"

#include "simulator.h"

Simulator::Simulator()
    : state(SIMULATOR_STATE_IDLE), overlay(&physicsEngine)
{
}

Simulator::~Simulator()
{
  glfwTerminate();
}

void Simulator::Start()
{
  if (state == SIMULATOR_STATE_IDLE)
  {
    state = SIMULATOR_STATE_PAUSED;
    clock.Start();
    std::cout << "Simulator started." << std::endl;
  }
  else
  {
    std::cerr << "Simulator is already running or not in a valid state to start." << std::endl;
  }

  while (!glfwWindowShouldClose(window))
  {
    // Calculate delta time
    double deltaTime = std::min(clock.GetDeltaTime(), 0.05);

    // Process input
    ProcessInput();

    // Update physics engine
    Update(deltaTime);

    // Render the scene
    Render();
  }
}

GLFWwindow *Simulator::Init()
{
  if (!(window = initGLFW()))
  {
    std::cerr << "ERROR::SIMULATOR::INIT: Failed to set up GLFW window." << std::endl;
    return nullptr;
  }
  overlay.Init(window);
  physicsEngine.Init();
  loadResources();
  Shader particleShader = ResourceManager::GetShader("particleShader");
  particleRenderer = new Renderer(window, particleShader);
  return window;
}

void Simulator::ProcessInput()
{
  int displayWidth, displayHeight;
  glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

  if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    glfwSetWindowShouldClose(window, true);

  if (ImGui::IsKeyPressed(ImGuiKey_Enter))
    state = state == SimulatorState::SIMULATOR_STATE_PAUSED ? state = SimulatorState::SIMULATOR_STATE_RUNNING : state = SimulatorState::SIMULATOR_STATE_PAUSED;

  if (ImGui::IsKeyPressed(ImGuiKey_1))
    physicsEngine.AddParticle(0, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_2))
    physicsEngine.AddParticle(1, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());
  
  if (ImGui::IsKeyPressed(ImGuiKey_3))
    physicsEngine.AddParticle(2, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_4))
    physicsEngine.AddParticle(3, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_5))
    physicsEngine.AddParticle(4, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_6))
    physicsEngine.AddParticle(5, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_7))
    physicsEngine.AddParticle(6, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_8))
    physicsEngine.AddParticle(7, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_9))
    physicsEngine.AddParticle(8, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

  if (ImGui::IsKeyPressed(ImGuiKey_0))
    physicsEngine.AddParticle(9, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());
}

void Simulator::Update(float delta)
{
  if (state == SIMULATOR_STATE_RUNNING)
  {
    physicsEngine.Update(delta, window);
  }
}

void Simulator::Render()
{
  // Rendering
  int displayWidth, displayHeight;
  glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
  glClearColor(0.0f, 0.42f, 0.0f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);
  particleRenderer->Render(physicsEngine.positionsInSSBO, Configurations::particleRadius, physicsEngine.colors, physicsEngine.particleCount);
  physicsEngine.swapFence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
  // ImGui
  overlay.HandleInput();
  overlay.Render();

  // Call & Swap
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam)
{
    std::cerr << "OpenGL Debug Message [" << id << "]: " << message << "\n";

    // Optional: Print extra context
    std::cerr << "    Source: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API:             std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Other"; break;
    }

    std::cerr << "\n    Type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "Other"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cerr << "Marker"; break;
    }

    std::cerr << "\n    Severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Notification"; break;
    }

    std::cerr << "\n\n";
}

GLFWwindow *Simulator::initGLFW()
{
  if (!glfwInit())
    return nullptr;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(STARTING_WINDOW_WIDTH, STARTING_WINDOW_HEIGHT, "Particle Life++", NULL, NULL);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSwapInterval(0);

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

  glViewport(0, 0, STARTING_WINDOW_WIDTH, STARTING_WINDOW_HEIGHT);
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
                      0, nullptr, GL_FALSE);
  glDebugMessageCallback(GLDebugMessageCallback, nullptr);

  return window;
}

void Simulator::loadResources()
{
  Shader circleShader = ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", nullptr, "particleShader");
}