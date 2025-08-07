#ifndef RENDERER_H
#define RENDERER_H

#include <plpp/shader.h>
#include <glfw/glfw3.h>
#include <vector>

class Renderer
{
public:
  Renderer(GLFWwindow *window, Shader &shader);
  ~Renderer();

  void Render(const unsigned int positions, const float radius, const std::vector<glm::vec4> colors, const int particleCount);

  Shader &GetShader() { return shader; }

private:
  Shader shader;
  GLFWwindow *window;
  unsigned int quadVAO;
  unsigned int positionInstanceVBO;
  unsigned int colorInstanceVBO;
  void initRenderData();
};

#endif