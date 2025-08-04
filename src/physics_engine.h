#ifndef PHYSICS_H
#define PHYSICS_ENGINE_H

#include <glm/glm.hpp>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "thread_pool.h"

class PhysicsEngine
{
public:
  PhysicsEngine();
  ~PhysicsEngine();

  void Update(float deltaTime);
  void AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity);
  int GetParticleCount();
  int GetParticleType(int particle);
  glm::vec2 GetParticlePosition(int particle);
  glm::vec2 GetParticleVelocity(int particle);

private:
  int particleCount = 0;
  std::vector<int> typeIds;
  std::vector<glm::vec2> positions;
  std::vector<glm::vec2> velocities;

  void applyForces(int particle, const glm::vec2 &force, float deltaTime);
  glm::vec2 calculateForces(int particle);

  std::atomic<int> remaining;
  std::mutex mut;
  std::condition_variable condition;
  ThreadPool threadPool;

  void worker(int offset, int step, float deltaTime);
};

#endif