#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

// C++ Standard Library
#include <vector>

// Project Includes
#include "plpp/shader.h"

namespace PLPP
{
  class PhysicsEngine
  {
  public:
    PhysicsEngine();
    ~PhysicsEngine();

    void Init();
    void UpdateColors();
    void Update(float deltaTime, GLFWwindow *window);
    void AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity);

    int particleCount = 0;
    std::vector<int> typeIds;
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> velocities;
    std::vector<glm::vec4> colors;

    unsigned int positionsInSSBO, positionsOutSSBO, velocitySSBO, typeSSBO, forcesSSBO;
    glm::vec2 *positionsInPtr;
    glm::vec2 *positionsOutPtr;
    glm::vec2 *velocitiesPtr;
    int *typesPtr;
    float *forcesPtr;
    GLsync swapFence = nullptr;

  private:
    Shader computeShader;
    std::vector<glm::vec4 *> colorPointers;
    void applyForces(int particle, const glm::vec2 &force, float deltaTime);
    glm::vec2 calculateForces(int particle);
  };
}

#endif