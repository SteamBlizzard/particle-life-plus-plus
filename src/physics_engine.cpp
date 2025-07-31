#include <thread>
#include <chrono>

#include "physics_engine.h"

#include <iostream>

void PhysicsEngine::AddParticle(const Particle &particle)
{
    // Add a particle to the simulation
    particles.push_back(new Particle(particle));
}

void PhysicsEngine::Update(float deltaTime)
{
    // Update the physics state of particles
    for (Particle *particle : particles)
    {
        glm::vec2 forces = calculateForces(*particle);
        applyForces(*particle, forces, deltaTime);
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
    return glm::vec2(0.0f, 0.0f);
}