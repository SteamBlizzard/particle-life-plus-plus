#include "plpp/physics_engine.h"

// Project Includes
#include "plpp/settings.h"
#include "plpp/resource_manager.h"

// External Libraries
#include <glad/glad.h>

// C++ Standard Library
#include <format>
#include <iostream>

namespace PLPP
{
  PhysicsEngine::PhysicsEngine(Shader computeShader) : computeShader_(computeShader)
  {
    unsigned int storageFlags = GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT;
    unsigned int accessFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    glGenBuffers(1, &positionsInSSBO_);
    glGenBuffers(1, &positionsOutSSBO_);
    glGenBuffers(1, &velocitySSBO_);
    glGenBuffers(1, &typeSSBO_);
    glGenBuffers(1, &forcesSSBO_);

    size_t positionSize = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
    size_t velocitySize = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
    size_t typeSize = sizeof(int) * MAXIMUM_PARTICLES;
    size_t forceSize = sizeof(float) * MAXIMUM_PARTICLE_TYPES * MAXIMUM_PARTICLE_TYPES;

    // Input positions buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionsInSSBO_);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, positionSize, nullptr, storageFlags);
    positionsInPtr_ = reinterpret_cast<glm::vec2 *>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, positionSize, accessFlags));
    if (!positionsInPtr_)
      std::cerr << "Failed to map positionsIn buffer!\n";

    // Output positions buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionsOutSSBO_);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, positionSize, nullptr, storageFlags);
    positionsOutPtr_ = reinterpret_cast<glm::vec2 *>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, positionSize, accessFlags));
    if (!positionsOutPtr_)
      std::cerr << "Failed to map positionsOut buffer!\n";

    // Velocities buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO_);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, velocitySize, nullptr, storageFlags);
    velocitiesPtr_ = reinterpret_cast<glm::vec2 *>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, velocitySize, accessFlags));
    if (!velocitiesPtr_)
      std::cerr << "Failed to map velocities buffer!\n";

    // Types buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, typeSSBO_);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, typeSize, nullptr, storageFlags);
    typesPtr_ = reinterpret_cast<int *>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, typeSize, accessFlags));
    if (!typesPtr_)
      std::cerr << "Failed to map types buffer!\n";

    // Forces buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, forcesSSBO_);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, forceSize, nullptr, storageFlags);
    forcesPtr_ = reinterpret_cast<float *>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, forceSize, accessFlags));
    if (!forcesPtr_)
      std::cerr << "Failed to map forces buffer!\n";

    memcpy(positionsOutPtr_, positionsInPtr_, sizeof(glm::vec2) * MAXIMUM_PARTICLES);
  }

  void PhysicsEngine::AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity)
  {
    if (particleCount >= MAXIMUM_PARTICLES)
      return;

    positionsInPtr_[particleCount] = position;
    velocitiesPtr_[particleCount] = velocity;
    typesPtr_[particleCount] = typeId;
    colorPointers_.push_back(&particleColors[typeId]);
    colors.push_back(particleColors[typeId]);

    particleCount++;
  }

  void PhysicsEngine::UpdateColors()
  {
    for (int i = 0; i < colorPointers_.size(); i++)
    {
      colors[i] = *colorPointers_[i];
    }
  }

  void PhysicsEngine::Update(float deltaTime, GLFWwindow *window)
  {
    if (particleCount > 0)
    {
      int displayWidth, displayHeight;
      glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

      computeShader_.Use();
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionsInSSBO_);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionsOutSSBO_);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocitySSBO_);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, typeSSBO_);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, forcesSSBO_);

      computeShader_.SetFloat("delta", deltaTime);
      computeShader_.SetFloat("particleRadius", particleRadius);
      computeShader_.SetVec2i("display", displayWidth, displayHeight);
      computeShader_.SetInteger("particleCount", particleCount);
      computeShader_.SetFloat("friction", friction);
      computeShader_.SetFloat("gravityRadius", effectiveForceRadius);
      computeShader_.SetFloat("forceMultiplier", forceMultiplier);
      computeShader_.SetInteger("maxTypeCount", MAXIMUM_PARTICLE_TYPES);

      computeShader_.Dispatch((particleCount + 255) / 256);
      for (int i = 0; i < particleCount; ++i)
      {
        if (!std::isfinite(positionsOutPtr_[i].x) || !std::isfinite(positionsOutPtr_[i].y))
        {
          std::cerr << "Particle " << i << " has invalid position: "
                    << positionsOutPtr_[i].x << ", " << positionsOutPtr_[i].y << std::endl;
        }
      }
      if (swapFence_)
      {
        glClientWaitSync(swapFence_, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        glDeleteSync(swapFence_);
      }
      std::swap(positionsInSSBO_, positionsOutSSBO_);
      std::swap(positionsInPtr_, positionsOutPtr_);
    }
  }

  float &PhysicsEngine::GetForceValue(int typeIdActed, int typeIdActing)
  {
    return PhysicsEngine::forceMatrix[typeIdActed * MAXIMUM_PARTICLE_TYPES + typeIdActing];
  };

  void PhysicsEngine::applyForces(int particle, const glm::vec2 &force, float deltaTime)
  {
    glm::vec2 velocity = velocities[particle];
    velocities[particle] += (force * deltaTime) - (velocity * friction * deltaTime);
  }

  glm::vec2 PhysicsEngine::calculateForces(int particle)
  {
    glm::vec2 finalForce = glm::vec2();

    glm::vec2 particlePosition = positions[particle];
    for (int i = 0; i < particleCount; i++)
    {
      if (i == particle)
        continue;
      float force = GetForceValue(typeIds[particle], typeIds[i]);

      glm::vec2 forceVector = normalize(positions[i] - particlePosition);
      forceVector *= force * forceMultiplier;
      finalForce += forceVector;
    }
    return finalForce;
  }
}