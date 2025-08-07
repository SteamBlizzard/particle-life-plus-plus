#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

#include <plpp/renderer.h>
#include <plpp/constants.h>

Renderer::Renderer(GLFWwindow *window, Shader &shader)
    : window(window), shader(shader)
{
  initRenderData();
}

Renderer::~Renderer()
{
  glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::initRenderData()
{
  unsigned int VBO;
  float vertices[] = {
      // x, y
      -1.0f, 1.0f,
      1.0f, -1.0f,
      -1.0f, -1.0f,
      1.0f, 1.0f};

  unsigned int EBO;
  unsigned int indices[] = {
      0, 1, 2,
      0, 1, 3};

  // Instantiate Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &positionInstanceVBO);
  glGenBuffers(1, &colorInstanceVBO);

  // Bind VAO and set vertex attribute pointers
  glBindVertexArray(quadVAO);

  // Bind VBO and copy vertex data to it
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind EBO and copy index data to it
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Define vertex attribute structure
  // (position 0, 2 floats per vertex, no normalization, stride of 2 floats, offset 0)
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  // Unbind the VAO (rebind when rendering)
  glBindVertexArray(0);
}

void Renderer::Render(
    const unsigned int positions,
    const float radius,
    const std::vector<glm::vec4> colors,
    const int particleCount
  )
{
  // Use the shader program
  shader.Use();

  glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positions);

  int displayWidth, displayHeight;
  glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
  shader.SetVec2f("u_resolution", displayWidth, displayHeight);

  glm::mat4 projection = glm::ortho(0.0f, (float)displayWidth, (float)displayHeight, 0.0f, -1.0f, 1.0f);
  shader.SetMat4("projection", projection);

  shader.SetFloat("radius", radius);

  // Bind the VAO and draw the quad
  glBindVertexArray(quadVAO);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particleCount);
  // Unbind the VAO
  glBindVertexArray(0);
}