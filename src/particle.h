#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle
{
  public:
    int type; // Type of particle (defined by particle manager matrix)
    glm::vec2 position; // x, y position components
    glm::vec2 velocity; // x, y velocity components

    // Constructor to initialize particle properties
    Particle(int particleType, glm::vec2 pos, glm::vec2 vel)
        : type(particleType)
    {
        position.x = pos.x;
        position.y = pos.y;
        velocity.x = vel.x;
        velocity.y = vel.y;
    }
};

#endif