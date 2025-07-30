#ifndef PHYSICS_H
#define PHYSICS_H

#include <particle.h>
#include <glm/glm.hpp>

class Physics
{
  public:
    Physics() = default;
    ~Physics() = default;

    // Function to update the physics state
    void update(float deltaTime);

    // Function to add a particle to the simulation
    void addParticle(const Particle &particle);
    
  private:
    void applyForces(Particle &particle, const glm::vec2 &force, float deltaTime);
    std::vector<Particle> particles; // List of particles in the simulation

    glm::vec2 calculateForces(const Particle &particle);
};

#endif