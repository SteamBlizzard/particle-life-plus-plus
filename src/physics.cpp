#include <thread>
#include <chrono>

#include "physics.h"

void Physics::addParticle(const Particle &particle)
{
    // Add a particle to the simulation
    particles.push_back(particle);
}

void Physics::update(float deltaTime)
{
    // Update the physics state of particles
    for (auto &particle : particles)
    {
        glm::vec2 forces = calculateForces(particle);
        applyForces(particle, forces, deltaTime);
    }
}

void Physics::applyForces(Particle &particle, const glm::vec2 &force, float deltaTime)
{
    // Apply forces to the particle
    particle.velocity += force * deltaTime;
    particle.position += particle.velocity * deltaTime;
}