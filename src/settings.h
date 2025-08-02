#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <utility>

class Settings
{
public:
  static std::pair<int, int> getResolution();
  static int GetParticleCountLimit();
  static int GetParticleTypeLimit();
  static float GetParticleSize();
  static std::vector<std::vector<float>> GetForceMatrix();
  static float GetForceValue(int typeIdActed, int typeIdActing);
  static float GetForceValues(int typeId);

  static void SetResolution(int width, int height);
  static void SetParticleCountLimit();
  static void SetParticleTypeLimit();
  static void SetParticleSize();
  static void SetForce(int typeIdActed, int typeIdActing, float force);

private:
  Settings() {}
  // Represents "particle <x> feels a force of [x,y] from particle <y>"
  static std::vector<std::vector<float>> forceMatrix;
};

#endif