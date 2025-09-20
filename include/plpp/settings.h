#ifndef SETTINGS_H
#define SETTINGS_H

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

// C++ Standard Library
#include <array>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace PLPP
{
  class Settings
  {
  public:
    static constexpr std::array<std::pair<int, int>, 9> RESOLUTIONS = {
        std::pair<int, int>(1280, 720),
        std::pair<int, int>(1280, 800),
        std::pair<int, int>(1440, 900),
        std::pair<int, int>(1600, 1200),
        std::pair<int, int>(1920, 1080),
        std::pair<int, int>(2048, 1536),
        std::pair<int, int>(2560, 1440),
        std::pair<int, int>(2560, 1600),
        std::pair<int, int>(3840, 2160)};

    inline static const std::array<std::string, 3> DISPLAY_MODES = {
        "Windowed",
        "Borderless",
        "Fullscreen"
    };

    inline static bool vsync = false;

    static std::pair<int, int> GetResolution(GLFWwindow *window);
    static std::string GetDisplayMode(GLFWwindow *window);
    static int GetParticleCountLimit();
    static int GetParticleTypeLimit();
    static std::vector<std::vector<float>> GetForceMatrix();

    static void SetResolution(GLFWwindow *window, std::pair<int, int> resolution);
    static void setDisplayMode(GLFWwindow *window, std::string displayMode);
    static void SetParticleCountLimit();
    static void SetParticleTypeLimit();
    static void SetForce(int typeIdActed, int typeIdActing, float force);

  private:
    Settings() {}
  };
}

#endif