#include <plpp/configurations.h>

float &Configurations::GetForceValue(int typeIdActed, int typeIdActing)
{
  return Configurations::forceMatrix[typeIdActed * MAXIMUM_PARTICLE_TYPES + typeIdActing];
};