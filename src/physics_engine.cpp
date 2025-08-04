#include <thread>
#include <chrono>

#include "physics_engine.h"
#include "settings.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

PhysicsEngine::PhysicsEngine()
    : threadPool(ThreadPool(std::thread::hardware_concurrency()))
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::AddParticle(const Particle &particle)
{
  // Add a particle to the simulation
  particles.push_back(new Particle(particle));
}

void PhysicsEngine::Update(float deltaTime)
{
  std::vector<std::thread> threads;
  remaining = Settings::threadCount;

  int particlesPerThread = (particles.size() + Settings::threadCount - 1) / Settings::threadCount;

  for (int i = 0; i < Settings::threadCount; ++i)
  {
    int start = i * particlesPerThread;
    int end = std::min((i + 1) * particlesPerThread, (int)particles.size());
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

  for (Particle *particle : particles)
  {
    particle->position += particle->velocity * deltaTime; // Update position based on velocity
  }
}

void PhysicsEngine::applyForces(Particle &particle, const glm::vec2 &force, float deltaTime)
{
  particle.velocity += force * deltaTime;
}

glm::vec2 PhysicsEngine::calculateForces(const Particle &particle)
{
  // Placeholder for force calculation logic
  return glm::vec2(-20.0f, -20.0f);
}

void PhysicsEngine::worker(int start, int end, float deltaTime)
{
  for (int i = start; i < end; i++)
  {
    Particle *particle = particles[i];
    glm::vec2 forces = calculateForces(*particle);
    applyForces(*particle, forces, deltaTime);
  }

  if (--remaining == 0)
  {
    std::lock_guard<std::mutex> lock(mut);
    condition.notify_one();
  }
}