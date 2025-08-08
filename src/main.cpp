// Project includes
#include "plpp/simulator.h"

int main()
{
  PLPP::Simulator &simulator = PLPP::Simulator::GetInstance();
  
  simulator.Start();

  return 0;
}