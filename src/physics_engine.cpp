#include <glad/glad.h>

#include <chrono>

#include <plpp/physics_engine.h>
#include <plpp/settings.h>
#include <plpp/configurations.h>
#include <plpp/resource_manager.h>

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <format>

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::Init()
{
  unsigned int storageFlags = GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT;
  unsigned int accessFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
  computeShader = ResourceManager::LoadShader("shaders/particles.comp", "computeShader");
  glGenBuffers(1, &positionsInSSBO);
  glGenBuffers(1, &positionsOutSSBO);
  glGenBuffers(1, &velocitySSBO);
  glGenBuffers(1, &typeSSBO);
  glGenBuffers(1, &forcesSSBO);

  size_t positionSize = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
  size_t velocitySize = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
  size_t typeSize     = sizeof(int) * MAXIMUM_PARTICLES;
  size_t forceSize    = sizeof(float) * MAXIMUM_PARTICLE_TYPES * MAXIMUM_PARTICLE_TYPES;

  // Input positions buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionsInSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, positionSize, nullptr, storageFlags);
  positionsInPtr = reinterpret_cast<glm::vec2*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, positionSize, accessFlags));
  if (!positionsInPtr)
    std::cerr << "Failed to map positionsIn buffer!\n";

  // Output positions buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionsOutSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, positionSize, nullptr, storageFlags);
  positionsOutPtr = reinterpret_cast<glm::vec2*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, positionSize, accessFlags));
  if (!positionsOutPtr)
    std::cerr << "Failed to map positionsOut buffer!\n";

  // Velocities buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, velocitySize, nullptr, storageFlags);
  velocitiesPtr = reinterpret_cast<glm::vec2*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, velocitySize, accessFlags));
  if (!velocitiesPtr) 
    std::cerr << "Failed to map velocities buffer!\n";

  // Types buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, typeSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, typeSize, nullptr, storageFlags);
  typesPtr = reinterpret_cast<int*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, typeSize, accessFlags));
  if (!typesPtr)
    std::cerr << "Failed to map types buffer!\n";

  // Forces buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, forcesSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, forceSize, nullptr, storageFlags);
  forcesPtr = reinterpret_cast<float*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, forceSize, accessFlags));
  if (!forcesPtr)
    std::cerr << "Failed to map forces buffer!\n";

  memcpy(positionsOutPtr, positionsInPtr, sizeof(glm::vec2) * MAXIMUM_PARTICLES);
}

void PhysicsEngine::AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity)
{
  if (particleCount >= MAXIMUM_PARTICLES)
    return;

  positionsInPtr[particleCount] = position;
  velocitiesPtr[particleCount] = velocity;
  typesPtr[particleCount] = typeId;
  colorPointers.push_back(&Configurations::particleColors[typeId]);
  colors.push_back(Configurations::particleColors[typeId]);

  particleCount++;
}

void PhysicsEngine::UpdateColors()
{
  for (int i = 0; i < colorPointers.size(); i++)
  {
    colors[i] = *colorPointers[i];
  }
}

void PhysicsEngine::Update(float deltaTime, GLFWwindow *window)
{
  if (particleCount > 0) {
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

    computeShader.Use();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionsInSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionsOutSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocitySSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, typeSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, forcesSSBO);

    computeShader.SetFloat("delta", deltaTime);
    computeShader.SetFloat("particleRadius", Configurations::particleRadius);
    computeShader.SetVec2i("display", displayWidth, displayHeight);
    computeShader.SetInteger("particleCount", particleCount);
    computeShader.SetFloat("friction", Configurations::friction);
    computeShader.SetFloat("gravityRadius", Configurations::gravityRadius);
    computeShader.SetFloat("forceMultiplier", Configurations::forceMultiplier);
    computeShader.SetInteger("maxTypeCount", MAXIMUM_PARTICLE_TYPES);

    computeShader.Dispatch((particleCount + 255) / 256);
    for (int i = 0; i < particleCount; ++i) {
      if (!std::isfinite(positionsOutPtr[i].x) || !std::isfinite(positionsOutPtr[i].y)) {
        std::cerr << "Particle " << i << " has invalid position: "
                  << positionsOutPtr[i].x << ", " << positionsOutPtr[i].y << std::endl;
      }
    }
    if (swapFence)
    {
      glClientWaitSync(swapFence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
      glDeleteSync(swapFence);
    }
    std::swap(positionsInSSBO, positionsOutSSBO);
    std::swap(positionsInPtr, positionsOutPtr);
  }
}

void PhysicsEngine::applyForces(int particle, const glm::vec2 &force, float deltaTime)
{
  glm::vec2 velocity = velocities[particle];
  velocities[particle] += (force * deltaTime) - (velocity * Configurations::friction * deltaTime);
}

glm::vec2 PhysicsEngine::calculateForces(int particle)
{
  glm::vec2 finalForce = glm::vec2();

  glm::vec2 particlePosition = positions[particle];
  for (int i = 0; i < particleCount; i++)
  {
    if (i == particle)
      continue;
    float force = Configurations::GetForceValue(typeIds[particle], typeIds[i]);

    glm::vec2 forceVector = normalize(positions[i] - particlePosition);
    forceVector *= force * Configurations::forceMultiplier;
    finalForce += forceVector;
  }
  return finalForce;
}

void PhysicsEngine::worker(int start, int end, float deltaTime)
{
  for (int i = start; i < end; i++)
  {
    glm::vec2 forces = calculateForces(i);
    applyForces(i, forces, deltaTime);
  }

  if (--remaining == 0)
  {
    std::lock_guard<std::mutex> lock(mut);
    condition.notify_one();
  }
}