#ifndef CLOCK_H
#define CLOCK_H

namespace PLPP
{
  class Clock
  {
  public:
    Clock() = default;
    ~Clock() = default;

    void Start();
    void Stop();
    double GetElapsedTime() const;
    double GetDeltaTime();

  private:
    double startTime_;
    double endTime_;
    double lastTime_;
    bool running_;
  };
}
#endif