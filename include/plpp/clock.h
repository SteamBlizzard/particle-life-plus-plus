#ifndef CLOCK_H
#define CLOCK_H

namespace PLPP
{
  class Clock
  {
  public:
    Clock();
    ~Clock();

    void Start();
    void Stop();
    double GetElapsedTime() const;
    double GetDeltaTime();

  private:
    double startTime;
    double endTime;
    double lastTime;
    bool running;
  };
}
#endif