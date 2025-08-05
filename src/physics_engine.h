#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <glm/glm.hpp>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "thread_pool.h"
#include "shader.h"

class PhysicsEngine
{
public:
  PhysicsEngine();
  ~PhysicsEngine();

  void Init();
  void Update(float deltaTime);
  void UpdateColors();
  void AddParticle(int typeId, glm::vec2 position, glm::vec2 velocity);

  void UpdateNew(float deltaTime);
  void AddParticleNew(int typeId, glm::vec2 position, glm::vec2 velocity);

  int particleCount = 0;
  std::vector<int> typeIds;
  std::vector<glm::vec2> positions;
  std::vector<glm::vec2> velocities;
  std::vector<glm::vec4> colors;

  unsigned int positionsInSSBO, positionsOutSSBO, velocitySSBO, typeSSBO, forcesSSBO, colorsSSBO;
  void* positionsInPtr;
  void* positionsOutPtr;
  void* velocitiesPtr;
  void* typesPtr;
  void* forcesPtr;
  void* colorsPtr;

private:
  Shader computeShader;
  std::vector<glm::vec4*> colorPointers;
  void applyForces(int particle, const glm::vec2 &force, float deltaTime);
  glm::vec2 calculateForces(int particle);

  std::atomic<int> remaining;
  std::mutex mut;
  std::condition_variable condition;
  ThreadPool threadPool;

  

  void worker(int offset, int step, float deltaTime);
};

#endif