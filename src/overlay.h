#ifndef OVERLAY_H
#define OVERLAY_H

#include <GLFW/glfw3.h>

class Overlay
{
public:
  Overlay();
  ~Overlay();

  void Init(GLFWwindow *window);
  void Render();
  void HandleInput();

private:
  GLFWwindow *window;

  bool mainMenuBarEnabled = true;
  bool configurationMenuEnabled = true;
  bool settingsMenuEnabled = false;

  void ShowMainMenuBar();
  void ShowConfigurationMenu();
  void ShowSettingsMenu();
};

#endif