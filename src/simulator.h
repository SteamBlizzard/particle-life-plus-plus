#ifndef SIMULATOR_H
#define SIMULATOR_H

enum SimulatorState
{
  SIMULATOR_STATE_IDLE,
  SIMULATOR_STATE_RUNNING,
  SIMULATOR_STATE_PAUSED,
  SIMULATOR_STATE_STOPPED
};

class Simulator
{
public:
  SimulatorState state;
  bool keys[1024];
  unsigned int width, height;
  Simulator(unsigned int width, unsigned int height);
  ~Simulator();
  void Init();
  void ProcessInput(float delta);
  void Update(float delta);
  void Render();
};

#endif