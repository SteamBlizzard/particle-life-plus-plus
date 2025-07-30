#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <glfw/glfw3.h>
#include "physics_engine.h"

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
  bool keys[1024];
  unsigned int width, height;
  GLFWwindow *window;
  PhysicsEngine physicsEngine;

  Simulator(unsigned int width, unsigned int height);
  ~Simulator();
  GLFWwindow *Init();
  void ProcessInput(float delta);
  void Update(float delta);
  void Render();

private:
  void loadResources();
  GLFWwindow *initGLFW();
  int initImGui(GLFWwindow *window);
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
#endif