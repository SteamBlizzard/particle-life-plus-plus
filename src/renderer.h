#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include <glfw/glfw3.h>

class Renderer
{
public:
  Renderer(GLFWwindow *window, Shader &shader);
  ~Renderer();

  void render(
      const glm::vec2 position,
      const glm::vec2 size = glm::vec2(5.0f, 5.0f),
      float rotation = 0.0f,
      const glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

  Shader &getShader() { return shader; }

private:
  Shader shader;
  GLFWwindow *window;
  unsigned int quadVAO;
  void initRenderData();
};

#endif