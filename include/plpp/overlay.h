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

    bool mainMenuBarEnabled = true;
    bool settingsAndConfigsMenuEnabled = false;

    void Render();

  private:
    GLFWwindow *window;
    PhysicsEngine &physicsEngine_;

    int typeCount_ = 1;
    int currentResolution_ = 0;
    int currentDisplayMode_ = 0;
    bool vsync_;

    void showMainMenuBar();
    void showSettingsAndConfigsMenu();

    void configurationMenu();
    void settingsMenu();

    void drawCircle(int particleId);
  };
}

#endif