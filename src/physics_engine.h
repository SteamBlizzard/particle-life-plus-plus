#ifndef PHYSICS_H
#define PHYSICS_ENGINE_H

#include <glm/glm.hpp>
#include <vector>

#include "particle.h"

class PhysicsEngine
{
public:
  PhysicsEngine() = default;
  ~PhysicsEngine() = default;

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

  glm::vec2 calculateForces(const Particle &particle);
};

#endif