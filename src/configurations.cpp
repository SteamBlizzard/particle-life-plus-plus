#include "configurations.h"

float Configurations::GetForceValue(int typeIdActed, int typeIdActing)
{
  return Configurations::forceMatrix[typeIdActed][typeIdActing];
};

std::vector<float> &Configurations::GetForceValues(int typeIdActed)
{
  return Configurations::forceMatrix[typeIdActed];
};