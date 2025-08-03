#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include <vector>
#include "constants.h"

class Configurations {
public:
  // Get a single value representing the force applied to one particle type from another
  static float GetForceValue(int typeIdActed, int typeIdActing);

  static void SetForceValue(int typeIdActed, int typeIdActing, float force);

  // Get all forces applied to one particle type from all others
  static std::vector<float> &GetForceValues(int typeIdActed);

private:
  // Represents "particle <x> feels a force of [x,y] from particle <y>"
  inline static std::vector<std::vector<float>> forceMatrix = std::vector<std::vector<float>>(MAXIMUM_PARTICLE_TYPES, std::vector<float>(MAXIMUM_PARTICLE_TYPES));
};

#endif