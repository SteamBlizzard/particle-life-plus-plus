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
#include "clock.h"
#include "constants.h"
#include "particle.h"
#include "renderer.h"
#include "resource_manager.h"
#include "shader.h"
#include "simulator.h"
#include "configurations.h"

Simulator::Simulator() : state(SIMULATOR_STATE_IDLE)
{
  physicsEngine = PhysicsEngine();
}

Simulator::~Simulator()
{
  glfwTerminate();
}

void Simulator::Start()
{
  if (state == SIMULATOR_STATE_IDLE)
  {
    state = SIMULATOR_STATE_RUNNING;
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

    // Update physics engine
    Update(deltaTime);

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
  physicsEngine.AddParticle(Particle(0, glm::vec2(540.0f, 540.0f), glm::vec2(0.0f, 0.0f)));
  return window;
}

void Simulator::ProcessInput(float delta)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    physicsEngine.applyForces(*physicsEngine.GetParticles()[0], glm::vec2(-400.0f, 0.0f), delta);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    physicsEngine.applyForces(*physicsEngine.GetParticles()[0], glm::vec2(400.0f, 0.0f), delta);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    physicsEngine.applyForces(*physicsEngine.GetParticles()[0], glm::vec2(0.0f, 400.0f), delta);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    physicsEngine.applyForces(*physicsEngine.GetParticles()[0], glm::vec2(0.0f, -400.0f), delta);
  }
}

void Simulator::Update(float delta)
{
  if (state == SIMULATOR_STATE_RUNNING)
  {
    Particle *particle = physicsEngine.GetParticles()[0];
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
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glClearColor(0.0f, 0.42f, 0.0f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (physicsEngine.GetParticles().empty())
  {
    std::cout << "No particles to render." << std::endl;
  }
  else
  {
    circleRenderer.GetShader().SetVec2f("u_resolution", display_w, display_h);
    Particle *particle = physicsEngine.GetParticles()[0];
    circleRenderer.Render(glm::vec2(particle->position.x, particle->position.y), glm::vec2(100.0f, 100.0f), 0.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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