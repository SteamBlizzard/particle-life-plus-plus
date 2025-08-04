#include <thread>
#include <chrono>

#include "physics_engine.h"
#include "settings.h"
#include "configurations.h"

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

void PhysicsEngine::AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity)
{
  typeIds.push_back(typeId);
  positions.push_back(position);
  velocities.push_back(velocity);
  particleCount++;
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
    positions[i] += velocities[i] * deltaTime; // Update position based on velocity
  }
}

void PhysicsEngine::applyForces(int particle, const glm::vec2 &force, float deltaTime)
{
  glm::vec2 velocity = velocities[particle];
  velocities[particle] += (force * deltaTime) - (velocity * Configurations::friction * deltaTime);
}

glm::vec2 PhysicsEngine::calculateForces(int particle)
{
  std::vector<float> &forces = Configurations::GetForceValues(typeIds[particle]);
  glm::vec2 finalForce = glm::vec2();

  glm::vec2 particlePosition = positions[particle];

  for (int i = 0; i < particleCount; i++)
  {
    if (i == particle) continue;
    float force = forces[typeIds[i]];
    
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

int PhysicsEngine::GetParticleCount()
{
  return particleCount;
}

int PhysicsEngine::GetParticleType(int particle)
{
  return typeIds[particle];
}

glm::vec2 PhysicsEngine::GetParticlePosition(int particle)
{
  return positions[particle];
}

glm::vec2 PhysicsEngine::GetParticleVelocity(int particle)
{
  return velocities[particle];
}