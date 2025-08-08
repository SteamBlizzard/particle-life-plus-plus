#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

// Project Includes
#include "plpp/constants.h"

// External Libraries
#include <glm/glm.hpp>

// C++ Standary Library
#include <vector>

namespace PLPP
{
  class Configurations
  {
  public:
    // Get a single value representing the force applied to one particle type from another
    static float &GetForceValue(int typeIdActed, int typeIdActing);

    // Particle type colors
    inline static std::vector<glm::vec4> particleColors = std::vector<glm::vec4>(MAXIMUM_PARTICLE_TYPES, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Friction coefficient (1.0f == None, 0.0f == Maximum)
    inline static float friction = 0.7f;

    // Sizes of the particles
    inline static float particleRadius = 5.0f;

    // Force multiplier
    inline static float forceMultiplier = 10.0f;

    // Maximum affected distance
    inline static float gravityRadius = 50.0f;

  private:
    // Represents "particle <x> feels a force of [x,y] from particle <y>"
    // index calculation = length * x + y
    inline static std::vector<float> forceMatrix = std::vector<float>(MAXIMUM_PARTICLE_TYPES * MAXIMUM_PARTICLE_TYPES);
  };
}

#endif