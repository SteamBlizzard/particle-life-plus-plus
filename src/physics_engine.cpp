#include <thread>
#include <chrono>

#include "physics_engine.h"

void PhysicsEngine::addParticle(const Particle &particle)
{
    // Add a particle to the simulation
    particles.push_back(particle);
}

void PhysicsEngine::update(float deltaTime)
{
    // Update the physics state of particles
    for (auto &particle : particles)
    {
        glm::vec2 forces = calculateForces(particle);
        applyForces(particle, forces, deltaTime);
    }
}

void PhysicsEngine::applyForces(Particle &particle, const glm::vec2 &force, float deltaTime)
{
    // Apply forces to the particle
    particle.velocity += force * deltaTime;
    particle.position += particle.velocity * deltaTime;
}

glm::vec2 PhysicsEngine::calculateForces(const Particle &particle)
{
    // Placeholder for force calculation logic
    return glm::vec2(-0.001f, 0.0f);
}