#ifndef RENDERER_H
#define RENDERER_H

// Project Includes
#include "plpp/shader.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

// C++ Standary Library
#include <vector>

namespace PLPP
{
  class Renderer
  {
  public:
    Renderer(GLFWwindow *window, Shader shader);
    ~Renderer();

    void Render(const unsigned int positions, const float radius, const std::vector<glm::vec4> colors, const int particleCount);

  private:
    Shader shader_;
    GLFWwindow *window_;
    unsigned int quadVAO_;
    unsigned int positionInstanceVBO_;
    unsigned int colorInstanceVBO_;
    void initRenderData();
  };
}

#endif