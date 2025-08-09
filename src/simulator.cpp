#include "plpp/simulator.h"

// Project Includes
#include "plpp/clock.h"
#include "plpp/constants.h"
#include "plpp/resource_manager.h"
#include "plpp/shader.h"

// External Libraries
#include <glm/glm.hpp>
#include <imgui.h>

// C++ Standard Library
#include <iostream>
#include <string>

namespace PLPP
{
  namespace
  {
    void framebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
      glViewport(0, 0, width, height);
    }

    void APIENTRY glDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                                         GLenum severity, GLsizei length,
                                         const GLchar *message, const void *userParam)
    {
      std::cerr << "OpenGL Debug Message [" << id << "]: " << message << "\n";

      // Optional: Print extra context
      std::cerr << "    Source: ";
      switch (source)
      {
      case GL_DEBUG_SOURCE_API:
        std::cerr << "API";
        break;
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cerr << "Window System";
        break;
      case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cerr << "Shader Compiler";
        break;
      case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cerr << "Third Party";
        break;
      case GL_DEBUG_SOURCE_APPLICATION:
        std::cerr << "Application";
        break;
      case GL_DEBUG_SOURCE_OTHER:
        std::cerr << "Other";
        break;
      }

      std::cerr << "\n    Type: ";
      switch (type)
      {
      case GL_DEBUG_TYPE_ERROR:
        std::cerr << "Error";
        break;
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cerr << "Deprecated Behavior";
        break;
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cerr << "Undefined Behavior";
        break;
      case GL_DEBUG_TYPE_PORTABILITY:
        std::cerr << "Portability";
        break;
      case GL_DEBUG_TYPE_PERFORMANCE:
        std::cerr << "Performance";
        break;
      case GL_DEBUG_TYPE_OTHER:
        std::cerr << "Other";
        break;
      case GL_DEBUG_TYPE_MARKER:
        std::cerr << "Marker";
        break;
      }

      std::cerr << "\n    Severity: ";
      switch (severity)
      {
      case GL_DEBUG_SEVERITY_HIGH:
        std::cerr << "High";
        break;
      case GL_DEBUG_SEVERITY_MEDIUM:
        std::cerr << "Medium";
        break;
      case GL_DEBUG_SEVERITY_LOW:
        std::cerr << "Low";
        break;
      case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cerr << "Notification";
        break;
      }

      std::cerr << "\n\n";
    }
  }

  Simulator::Simulator()
      : state_(SimulatorState::Idle),
        window_(Init()),
        physicsEngine_(ResourceManager::LoadShader("res/shaders/particles.comp", "computeShader")),
        overlay_(window_, physicsEngine_),
        particleShader_(ResourceManager::LoadShader("res/shaders/particles.vert", "res/shaders/particles.frag", nullptr, "particleShader")) {}

  void Simulator::Start()
  {
    if (state_ == SimulatorState::Idle)
    {
      state_ = SimulatorState::Paused;
      clock_.Start();
      std::cout << "Simulator started." << std::endl;
    }
    else
    {
      std::cerr << "Simulator is already running or not in a valid state to start." << std::endl;
    }

    while (!glfwWindowShouldClose(window_))
    {
      double deltaTime = std::min(clock_.GetDeltaTime(), 0.05);

      ProcessInput();
      Update(deltaTime);
      Render();
    }
  }

  GLFWwindow *Simulator::Init()
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

    #ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(GLDebugMessageCallback, nullptr);
    #endif

    return window;
  }

  void Simulator::ProcessInput()
  {
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window_, &displayWidth, &displayHeight);

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
      glfwSetWindowShouldClose(window_, true);

    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
      state_ = state_ == SimulatorState::Paused ? SimulatorState::Running : SimulatorState::Paused;

    if (ImGui::IsKeyPressed(ImGuiKey_1))
      physicsEngine_.AddParticle(0, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_2))
      physicsEngine_.AddParticle(1, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_3))
      physicsEngine_.AddParticle(2, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_4))
      physicsEngine_.AddParticle(3, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_5))
      physicsEngine_.AddParticle(4, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_6))
      physicsEngine_.AddParticle(5, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_7))
      physicsEngine_.AddParticle(6, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_8))
      physicsEngine_.AddParticle(7, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_9))
      physicsEngine_.AddParticle(8, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());

    if (ImGui::IsKeyPressed(ImGuiKey_0))
      physicsEngine_.AddParticle(9, glm::vec2(std::rand() % displayWidth, std::rand() % displayHeight), glm::vec2());
  }

  void Simulator::Update(float delta)
  {
    if (state_ == SimulatorState::Running)
      physicsEngine_.Update(delta, window_);
  }

  void Simulator::Render()
  {
    // Rendering
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window_, &displayWidth, &displayHeight);
    glClearColor(0.0f, 0.21f, 0.0f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    particleShader_.Render(window_, physicsEngine_.GetParticlePositions(), physicsEngine_.particleRadius, physicsEngine_.colors, physicsEngine_.particleCount);
    GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    physicsEngine_.SetFence(fence);

    // ImGui
    overlay_.HandleInput();
    overlay_.Render();

    // Call & Swap
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}