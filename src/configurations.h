#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include <vector>
#include <glm/glm.hpp>
#include "constants.h"

class Configurations
{
public:
  // Get a single value representing the force applied to one particle type from another
  static float GetForceValue(int typeIdActed, int typeIdActing);

  // Get all forces applied to one particle type from all others
  static std::vector<float> &GetForceValues(int typeIdActed);

  // Particle type colors
  inline static std::vector<glm::vec4> particleColors = std::vector<glm::vec4>(MAXIMUM_PARTICLE_TYPES);

  // Friction coefficient (1.0f == None, 0.0f == Maximum)
  inline static float friction = 0.7f;

  // Sizes of the particles
  inline static float particleRadius = 25.0f;

  // Force multiplier
  inline static float forceMultiplier = 10.0f;

private:
  // Represents "particle <x> feels a force of [x,y] from particle <y>"
  inline static std::vector<std::vector<float>> forceMatrix = std::vector<std::vector<float>>(MAXIMUM_PARTICLE_TYPES, std::vector<float>(MAXIMUM_PARTICLE_TYPES));
};

#endif