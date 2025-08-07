// Project includes
#include "plpp/simulator.h"

int main()
{
  PLPP::Simulator &simulator = PLPP::Simulator::GetInstance();

  GLFWwindow *window = simulator.Init();

  if (!window)
  {
    return -1;
  }

  simulator.Start();

  return 0;
}