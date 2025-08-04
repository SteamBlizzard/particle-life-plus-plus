// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

// C++ Standard Library
#include <iostream>
#include <string>
#include <cmath>

// Project includes
#include "clock.h"
#include "constants.h"
#include "renderer.h"
#include "resource_manager.h"
#include "shader.h"
#include "simulator.h"
#include "configurations.h"

Simulator::Simulator() : state(SIMULATOR_STATE_IDLE)
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
    double deltaTime = clock.GetDeltaTime();

    // Process input
    ProcessInput(deltaTime);

    if (state == SIMULATOR_STATE_RUNNING)
    {
      // Update physics engine
      Update(deltaTime);
    }

    // Render the scene
    Render();
  }
}

GLFWwindow *Simulator::Init()
{
  if (!(this->window = initGLFW()))
  {
    std::cerr << "ERROR::SIMULATOR::INIT: Failed to set up GLFW window." << std::endl;
    return nullptr;
  }
  overlay.Init(window);
  loadResources();
  // Initialize the physics engine with a default particle (while testing)
  physicsEngine.AddParticle(0, glm::vec2(STARTING_WINDOW_WIDTH / 2, STARTING_WINDOW_HEIGHT / 2), glm::vec2(0.0f, 0.0f));
  return window;
}

void Simulator::ProcessInput(float delta)
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
    physicsEngine.Update(delta);
  }
  else
  {
    std::cerr << "Simulator is not in a running state." << std::endl;
  }
}

void Simulator::Render()
{
  Shader circleShader = ResourceManager::GetShader("circleShader");
  Renderer circleRenderer(window, circleShader);

  // Rendering
  int displayWidth, displayHeight;
  glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
  glClearColor(0.0f, 0.42f, 0.0f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (physicsEngine.GetParticleCount() == 0)
  {
    std::cout << "No particles to render." << std::endl;
  }
  else
  {
    circleRenderer.GetShader().SetVec2f("u_resolution", displayWidth, displayHeight);
    for (int i = 0; i < physicsEngine.GetParticleCount(); i++)
    {
      circleRenderer.Render(physicsEngine.GetParticlePosition(i), glm::vec2(Configurations::particleRadius), 0.0f, Configurations::particleColors[physicsEngine.GetParticleType(i)]);
    }
  }

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

GLFWwindow *Simulator::initGLFW()
{
  if (!glfwInit())
    return nullptr;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

  return window;
}

void Simulator::loadResources()
{
  Shader circleShader = ResourceManager::LoadShader("shaders/circle.vert", "shaders/circle.frag", nullptr, "circleShader");
}