#include "configurations.h"

float Configurations::GetForceValue(int typeIdActed, int typeIdActing) {
  return Configurations::forceMatrix[typeIdActed][typeIdActing];
};

void Configurations::SetForceValue(int typeIdActed, int typeIdActing, float force) {
  Configurations::forceMatrix[typeIdActed][typeIdActing] = force;
};

std::vector<float> &Configurations::GetForceValues(int typeIdActed) {
  return Configurations::forceMatrix[typeIdActed];
};