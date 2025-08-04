#ifndef PHYSICS_H
#define PHYSICS_ENGINE_H

#include <glm/glm.hpp>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "particle.h"
#include "thread_pool.h"

class PhysicsEngine
{
public:
  PhysicsEngine();
  ~PhysicsEngine();

  void Update(float deltaTime);
  void AddParticle(const Particle &particle);
  std::vector<Particle *> GetParticles() const
  {
    return particles;
  }

  // Private once the physics engine is fully implemented
  // For now, we keep it public for testing purposes
  // private:
  void applyForces(Particle &particle, const glm::vec2 &force, float deltaTime);
  std::vector<Particle *> particles; // List of particles in the simulation
  glm::vec2 calculateForces(Particle &particle);

private:
  std::atomic<int> remaining;
  std::mutex mut;
  std::condition_variable condition;
  ThreadPool threadPool;

  void worker(int offset, int step, float deltaTime);
};

#endif