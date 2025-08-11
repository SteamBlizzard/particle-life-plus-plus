#include "plpp/settings.h"

// C++ Standard Library
#include <format>
#include <iostream>

namespace PLPP
{
  std::pair<int, int> Settings::GetResolution(GLFWwindow *window)
  {
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

    return std::pair<int, int>(displayWidth, displayHeight);
  }

  void Settings::SetResolution(GLFWwindow *window, std::pair<int, int> resolution)
  {
    glfwSetWindowSize(window, resolution.first, resolution.second);
  }

  std::string Settings::GetDisplayMode(GLFWwindow *window)
  {
    if (glfwGetWindowMonitor(window) != nullptr)
    {
      return "Fullscreen";
    }

    int decorated = glfwGetWindowAttrib(window, GLFW_DECORATED);

    if (!decorated)
    {
      return "Borderless";
    }

    return "Windowed";
  }

  void Settings::setDisplayMode(GLFWwindow *window, std::string displayMode)
  {
    int currentWidth, currentHeight;
    glfwGetFramebufferSize(window, &currentWidth, &currentHeight);

    if (displayMode == "Windowed")
    {
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
      glfwSetWindowMonitor(window, nullptr, 0, 0, currentWidth, currentHeight, GLFW_DONT_CARE);
    }
    else if (displayMode == "Borderless")
    {
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      glfwSetWindowMonitor(window, nullptr, 0, 0, currentWidth, currentHeight, 0);
    }
    else if (displayMode == "Fullscreen")
    {
      GLFWmonitor *monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode *mode = glfwGetVideoMode(monitor);
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
      std::cerr << std::format("ERROR::SETTINGS::DISPLAY: Invalid Display Mode '{}'", displayMode) << std::endl;
    }
  }
}