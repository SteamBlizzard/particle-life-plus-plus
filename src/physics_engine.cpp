#include <glad/glad.h>

#include <thread>
#include <chrono>

#include "physics_engine.h"
#include "settings.h"
#include "configurations.h"
#include "resource_manager.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <format>

PhysicsEngine::PhysicsEngine()
    : threadPool(ThreadPool(std::thread::hardware_concurrency()))
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::Init()
{
  unsigned int flags = GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
  computeShader = ResourceManager::LoadShader("shaders/particles.compute", "computeShader");
  glGenBuffers(0, &positionsInSSBO);
  glGenBuffers(1, &positionsOutSSBO);
  glGenBuffers(2, &velocitySSBO);
  glGenBuffers(3, &forcesSSBO);
  glGenBuffers(4, &colorsSSBO);

  size_t positionSize = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
  size_t velocitySize = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
  size_t typeSize     = sizeof(unsigned int) * MAXIMUM_PARTICLES;
  size_t forceSize    = sizeof(glm::vec2) * MAXIMUM_PARTICLES;
  size_t colorsSize   = sizeof(glm::vec4) * MAXIMUM_PARTICLES;

  // Positions buffer 1
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionsInSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * MAXIMUM_PARTICLES, nullptr, flags);
  positionsInPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, positionSize, flags);

  // Positions buffer 2
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionsOutSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * MAXIMUM_PARTICLES, nullptr, flags);
  positionsOutPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, positionSize, flags);

  // Velocities buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * MAXIMUM_PARTICLES, nullptr, flags);
  velocitiesPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, velocitySize, flags);

  // Forces buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, forcesSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MAXIMUM_PARTICLES, nullptr, flags);
  forcesPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, forceSize, flags);

  // Forces buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, forcesSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MAXIMUM_PARTICLES, nullptr, flags);
  forcesPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, forceSize, flags);

  // Color buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorsSSBO);
  glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * MAXIMUM_PARTICLES, nullptr, flags);
  colorsPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, colorsSize, flags);
}

void PhysicsEngine::AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity)
{
  if (particleCount < MAXIMUM_PARTICLES)
  {
    typeIds.push_back(typeId);
    positions.push_back(position);
    velocities.push_back(velocity);
    colorPointers.push_back(&Configurations::particleColors[typeId]);
    colors.push_back(*colorPointers[particleCount++]);
  }
}

void PhysicsEngine::AddParticleNew(int typeId, glm::vec2 position, glm::vec2 velocity)
{
  particleCount++;
}

void PhysicsEngine::UpdateColors()
{
  for (int i = 0; i < colorPointers.size(); i++)
  {
    colors[i] = *colorPointers[i];
  }
}

void PhysicsEngine::Update(float deltaTime)
{
  std::vector<std::thread> threads;
  remaining = Settings::threadCount;

  int particlesPerThread = (particleCount + Settings::threadCount - 1) / Settings::threadCount;

  for (int i = 0; i < Settings::threadCount; ++i)
  {
    int start = i * particlesPerThread;
    int end = std::min((i + 1) * particlesPerThread, particleCount);
    if (start >= end)
    {
      remaining--;
      continue;
    }

    threadPool.Enqueue([start, end, deltaTime, this]()
                       { worker(start, end, deltaTime); });
  }

  std::unique_lock<std::mutex> lock(mut);
  condition.wait(lock, [&]
                 { return remaining == 0; });

  for (int i = 0; i < particleCount; i++)
  {
    positions[i] += velocities[i] * deltaTime;
  }
}

void PhysicsEngine::UpdateNew(float deltaTime)
{

  // Positions
  glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_DYNAMIC_COPY);
  
  // Velocities
  glBufferData(GL_SHADER_STORAGE_BUFFER, velocities.size() * sizeof(glm::vec2), velocities.data(), GL_DYNAMIC_COPY);

  // Types
  glBufferData(GL_SHADER_STORAGE_BUFFER, typeIds.size() * sizeof(int), typeIds.data(), GL_DYNAMIC_COPY);

  computeShader.Use();
  computeShader.SetFloat("delta", deltaTime);
  computeShader.SetInteger("particleCount", particleCount);
  computeShader.SetFloat("friction", Configurations::friction);
  computeShader.SetFloat("forceMultiplier", Configurations::forceMultiplier);
  computeShader.SetInteger("maxTypeCount", MAXIMUM_PARTICLE_TYPES);

  computeShader.Dispatch((particleCount + 255) / 256);
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