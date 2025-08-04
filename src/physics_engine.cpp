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
  particle.velocity += (force * deltaTime) - (particle.velocity * Configurations::friction * deltaTime);
}

glm::vec2 PhysicsEngine::calculateForces(Particle &particle)
{
  std::vector<float> &forces = Configurations::GetForceValues(particle.type);
  glm::vec2 finalForce = glm::vec2();

  for (auto actor : particles)
  {
    if (actor == &particle) continue;
    float force = forces[actor->type];
    
    glm::vec2 forceVector = normalize(actor->position - particle.position);
    forceVector *= force * Configurations::forceMultiplier;
    finalForce += forceVector;
  }
  return finalForce;
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