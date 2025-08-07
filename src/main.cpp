// C++ Standard Library
#include <iostream>
#include <string>
#include <cmath>

// Project includes
#include <plpp/constants.h>
#include <plpp/simulator.h>

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