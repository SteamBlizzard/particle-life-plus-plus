#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

// Project Includes
#include "constants.h"
#include "plpp/shader.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

// C++ Standard Library
#include <vector>

namespace PLPP
{
  class PhysicsEngine
  {
  public:
    int particleCount = 0;
    std::vector<int> typeIds;
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> velocities;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec4> particleColors = std::vector<glm::vec4>(MAXIMUM_PARTICLE_TYPES, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    std::vector<float> forceMatrix = std::vector<float>(MAXIMUM_PARTICLE_TYPES * MAXIMUM_PARTICLE_TYPES);
    
    // Represents "particle <x> feels a force of [x,y] from particle <y>"
    // index calculation = length * x + y
    float &GetForceValue(int typeIdActed, int typeIdActing);

    PhysicsEngine(Shader computeShader);
    ~PhysicsEngine() = default;

    void UpdateColors();
    void Update(float deltaTime, GLFWwindow *window);
    void AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity);

    // Friction coefficient (1.0f == None, 0.0f == Maximum)
    float friction = 0.7f;
    float particleRadius = 5.0f;
    float forceMultiplier = 10.0f;
    float effectiveForceRadius = 50.0f;
    
    float *GetForcesBuffer() { return forcesPtr_; }
    GLuint GetParticlePositions() { return positionsInSSBO_; }
    void SetFence(GLsync &fence) { swapFence_ = fence; }

  private:
    GLuint positionsInSSBO_, positionsOutSSBO_, velocitySSBO_, typeSSBO_, forcesSSBO_;
    glm::vec2 *positionsInPtr_;
    glm::vec2 *positionsOutPtr_;
    glm::vec2 *velocitiesPtr_;
    float *forcesPtr_;
    int *typesPtr_;
    
    Shader computeShader_;
    std::vector<glm::vec4*> colorPointers_;
    GLsync swapFence_ = nullptr;
    
    void applyForces(int particle, const glm::vec2 &force, float deltaTime);
    glm::vec2 calculateForces(int particle);
  };
}

#endif