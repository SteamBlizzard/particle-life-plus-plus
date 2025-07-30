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

    void update(float deltaTime);
    void addParticle(const Particle &particle);
    std::vector<Particle> getParticles() const
    {
        return particles;
    }
    
  private:
    void applyForces(Particle &particle, const glm::vec2 &force, float deltaTime);
    std::vector<Particle> particles; // List of particles in the simulation

    glm::vec2 calculateForces(const Particle &particle);
};

#endif