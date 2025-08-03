#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <glfw/glfw3.h>
#include "physics_engine.h"
#include "overlay.h"
#include "clock.h"

enum SimulatorState
{
  SIMULATOR_STATE_IDLE,
  SIMULATOR_STATE_RUNNING,
  SIMULATOR_STATE_PAUSED,
  SIMULATOR_STATE_STOPPED
};

class Simulator
{
public:
  SimulatorState state;
  GLFWwindow *window;
  PhysicsEngine physicsEngine;
  Overlay overlay;
  Clock clock;

  Simulator();
  ~Simulator();
  GLFWwindow *Init();
  void Start();
  void ProcessInput(float delta);
  void Update(float delta);
  void Render();

private:
  void loadResources();
  GLFWwindow *initGLFW();
  int initImGui(GLFWwindow *window);
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
#endif