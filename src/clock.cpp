#include "plpp/clock.h"

// External Libraries
#include <glad/glad.h>
#include <glfw/glfw3.h>

Clock::Clock()
    : startTime(0.0f), endTime(0.0f), lastTime(0.0f), running(false)
{
}

Clock::~Clock()
{
}

void Clock::Start()
{
  if (!running)
  {
    startTime, lastTime = glfwGetTime();
    running = true;
  }
}

void Clock::Stop()
{
  if (running)
  {
    endTime = glfwGetTime();
    running = false;
  }
}

double Clock::GetElapsedTime() const
{
  if (running)
  {
    return glfwGetTime() - startTime;
  }
  return endTime - startTime;
}

double Clock::GetDeltaTime()
{
  double currentTime = glfwGetTime();
  double deltaTime = currentTime - lastTime;
  lastTime = currentTime;
  return deltaTime;
}