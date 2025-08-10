#include "plpp/clock.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace PLPP
{
  void Clock::Start()
  {
    if (!running_)
    {
      startTime_, lastTime_ = glfwGetTime();
      running_ = true;
    }
  }

  void Clock::Stop()
  {
    if (running_)
    {
      endTime_ = glfwGetTime();
      running_ = false;
    }
  }

  double Clock::GetElapsedTime() const
  {
    if (running_)
    {
      return glfwGetTime() - startTime_;
    }
    return endTime_ - startTime_;
  }

  double Clock::GetDeltaTime()
  {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime_;
    lastTime_ = currentTime;
    return deltaTime;
  }
}