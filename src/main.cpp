// Project includes
#include "plpp/simulator.h"

int main()
{
  Simulator &simulator = Simulator::GetInstance();

  GLFWwindow *window = simulator.Init();
  
  if (!window)
  {
    return -1;
  }

  simulator.Start();

  return 0;
}