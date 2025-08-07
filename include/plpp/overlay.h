#ifndef OVERLAY_H
#define OVERLAY_H

#include <plpp/physics_engine.h>
#include <GLFW/glfw3.h>

class Overlay
{
public:
  Overlay(PhysicsEngine * physicsEngine) : physicsEngine(physicsEngine) {}
  ~Overlay();

  void Init(GLFWwindow *window);
  void Render();
  void HandleInput();

private:
  GLFWwindow *window;
  PhysicsEngine *physicsEngine;
  bool mainMenuBarEnabled = true;
  bool settingsAndConfigsMenuEnabled = false;

  void showMainMenuBar();
  void showSettingsAndConfigsMenu();

  void configurationMenu();
  void settingsMenu();
};

#endif