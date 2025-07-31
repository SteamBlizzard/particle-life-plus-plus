// C++ Standard Library
#include <iostream>
#include <string>
#include <cmath>

// Project includes
#include "constants.h"
#include "simulator.h"

int main()
{
    Simulator simulator(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    GLFWwindow *window = simulator.Init();
    if (!window)
    {
        return -1;
    }

    simulator.Start();
    
    simulator.~Simulator();

    return 0;
}