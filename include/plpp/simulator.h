#ifndef SIMULATOR_H
#define SIMULATOR_H

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

// Project Includes
#include "plpp/physics_engine.h"
#include "plpp/renderer.h"
#include "plpp/overlay.h"
#include "plpp/clock.h"



enum SimulatorState
{
  SIMULATOR_STATE_IDLE,
  SIMULATOR_STATE_RUNNING,
  SIMULATOR_STATE_PAUSED
};

class Simulator
{
public:
  public:
  SimulatorState state;
  GLFWwindow *window;
  PhysicsEngine physicsEngine;
  Overlay overlay;
  Clock clock;

  static Simulator& GetInstance()
  {
    static Simulator instance;
    return instance;
  }

  Simulator(const Simulator&) = delete;
  Simulator& operator=(const Simulator&) = delete;
  Simulator(Simulator&&) = delete;
  Simulator& operator=(Simulator&&) = delete;

  GLFWwindow *Init();
  void Start();
  void ProcessInput();
  void Update(float delta);
  void Render();

private:
  Simulator();
  ~Simulator();

  void loadResources();
  GLFWwindow *initGLFW();
  int initImGui(GLFWwindow *window);

  Renderer *particleRenderer;
};


#endif