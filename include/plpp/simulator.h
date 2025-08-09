#ifndef SIMULATOR_H
#define SIMULATOR_H

// Project Includes
#include "plpp/physics_engine.h"
#include "plpp/overlay.h"
#include "plpp/clock.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace PLPP
{
  class Simulator
  {
  public:
    static Simulator &GetInstance()
    {
      static Simulator instance;
      return instance;
    }

    void Start();

  private:
    enum class SimulatorState
    {
      Idle,
      Running,
      Paused
    };

    Simulator();
    ~Simulator() { glfwTerminate(); };

    void ProcessInput();
    void Update(float delta);
    void Render();

    GLFWwindow *Init();

    Simulator(const Simulator &) = delete;
    Simulator &operator=(const Simulator &) = delete;
    Simulator(Simulator &&) = delete;
    Simulator &operator=(Simulator &&) = delete;

    SimulatorState state_;
    GLFWwindow *window_;
    PhysicsEngine physicsEngine_;
    Overlay overlay_;
    Shader particleShader_;
    Clock clock_;
  };
}

#endif