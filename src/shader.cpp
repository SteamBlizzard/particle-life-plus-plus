#include "plpp/shader.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

// C++ Standard Library
#include <format>
#include <iostream>
#include <vector>

namespace PLPP
{
  Shader::Shader(const char *vertexSource, const char *fragmentSource)
  {
    unsigned int sVertex, sFragment;

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    
    initRenderData();
    type_ = ShaderType::Render;
  }

  Shader::Shader(const char *computeSource)
  {
    unsigned int sCompute;
    sCompute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(sCompute, 1, &computeSource, NULL);
    glCompileShader(sCompute);
    checkCompileErrors(sCompute, "COMPUTE");
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sCompute);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    glDeleteShader(sCompute);

    type_ = ShaderType::Compute;
  }

  Shader &Shader::Use()
  {
    glUseProgram(this->ID);
    return *this;
  }

  void Shader::Render(
      GLFWwindow *window,
      const unsigned int positions,
      const float radius,
      const std::vector<glm::vec4> colors,
      const int particleCount)
  {
    if (type_ != ShaderType::Render)
    {
      std::cerr << "ERROR::SHADER::RENDER: Attempting to render a shader of invalid type!" << std::endl;
      return;
    }
    // Use the shader program
    Use();

    glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO_);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positions);

    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    SetVec2f("u_resolution", displayWidth, displayHeight);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(displayWidth), static_cast<float>(displayHeight), 0.0f, -1.0f, 1.0f);
    SetMat4("projection", projection);

    SetFloat("radius", radius);

    glBindVertexArray(quadVAO_);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particleCount);

    glBindVertexArray(0);
  }

  void Shader::Dispatch(int groups)
  {
    if (type_ != ShaderType::Compute)
    {
      std::cerr << "ERROR::SHADER::COMPUTE: Attempting to dispatch a shader of invalid type!" << std::endl;
      return;
    }
      glDispatchCompute(groups, 1, 1);
      glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  }

  void Shader::SetFloat(const char *name, float value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
  }

  void Shader::SetInteger(const char *name, int value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
  }

  void Shader::SetVec2f(const char *name, float x, float y, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
  }

  void Shader::SetVec2f(const char *name, const glm::vec2 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
  }

  void Shader::SetVec3f(const char *name, float x, float y, float z, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
  }

  void Shader::SetVec3f(const char *name, const glm::vec3 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
  }
  
  void Shader::SetVec4f(const char *name, float x, float y, float z, float w, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
  }

  void Shader::SetVec4f(const char *name, const glm::vec4 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
  }

  void Shader::SetVec2i(const char *name, int x, int y, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform2i(glGetUniformLocation(this->ID, name), x, y);
  }

  void Shader::SetVec2i(const char *name, const glm::ivec2 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform2i(glGetUniformLocation(this->ID, name), value.x, value.y);
  }

  void Shader::SetVec3i(const char *name, int x, int y, int z, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform3i(glGetUniformLocation(this->ID, name), x, y, z);
  }
  
  void Shader::SetVec3i(const char *name, const glm::ivec3 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform3i(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
  }

  void Shader::SetVec4i(const char *name, int x, int y, int z, int w, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform4i(glGetUniformLocation(this->ID, name), x, y, z, w);
  }

  void Shader::SetVec4i(const char *name, const glm::ivec4 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform4i(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
  }
  
  void Shader::SetMat4(const char *name, const glm::mat4 &matrix, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void Shader::checkCompileErrors(unsigned int object, std::string type)
  {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(object, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(object, 1024, NULL, infoLog);
        std::cerr << std::format("ERROR::SHADER::{}::COMPILATION_FAILED\n{}", type, infoLog) << std::endl;
      }
    }
    else
    {
      glGetProgramiv(object, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(object, 1024, NULL, infoLog);
        std::cerr << std::format("ERROR::SHADER::{}::LINKING_FAILED\n{}", type, infoLog) << std::endl;
      }
    }
  }

  void Shader::initRenderData()
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
    glGenVertexArrays(1, &quadVAO_);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &positionInstanceVBO_);
    glGenBuffers(1, &colorInstanceVBO_);

    // Bind VAO and set vertex attribute pointers
    glBindVertexArray(quadVAO_);

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

    glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO_);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    // Unbind the VAO (rebind when rendering)
    glBindVertexArray(0);
  }

}