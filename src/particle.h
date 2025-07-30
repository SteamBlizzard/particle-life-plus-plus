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
    Particle(int particleType, float x, float y, float vx, float vy)
        : type(particleType)
    {
        position.x = x;
        position.y = y;
        velocity.x = vx;
        velocity.y = vy;
    }
};

#endif