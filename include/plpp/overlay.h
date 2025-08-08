#ifndef OVERLAY_H
#define OVERLAY_H

// Project Includes
#include "plpp/physics_engine.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace PLPP
{
  class Overlay
  {
  public:
    Overlay(GLFWwindow *window, PhysicsEngine &physicsEngine_);
    ~Overlay();

    void Render();
    void HandleInput();

  private:
    GLFWwindow *window;
    PhysicsEngine physicsEngine_;
    bool mainMenuBarEnabled = true;
    bool settingsAndConfigsMenuEnabled = false;

    void showMainMenuBar();
    void showSettingsAndConfigsMenu();

    void configurationMenu();
    void settingsMenu();
  };
}

#endif