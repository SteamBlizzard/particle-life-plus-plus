#ifndef SIMULATOR_H
#define SIMULATOR_H

// Project Includes
#include "plpp/physics_engine.h"
#include "plpp/renderer.h"
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
    void ProcessInput();
    void Update(float delta);
    void Render();

  private:
    Simulator();
    ~Simulator();

    enum SimulatorState
    {
      SIMULATOR_STATE_IDLE,
      SIMULATOR_STATE_RUNNING,
      SIMULATOR_STATE_PAUSED
    };
    
    Simulator(const Simulator &) = delete;
    Simulator &operator=(const Simulator &) = delete;
    Simulator(Simulator &&) = delete;
    Simulator &operator=(Simulator &&) = delete;
    
    GLFWwindow *Init();

    SimulatorState state;
    GLFWwindow *window;
    PhysicsEngine physicsEngine;
    Overlay overlay;
    Renderer particleRenderer;
    Clock clock;
    
    

  };
}

#endif