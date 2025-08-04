#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

#include "thread_pool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
  stop = false;
  for (size_t i = 0; i < numThreads; ++i)
  {
    workers.emplace_back([this]()
                         {
              while (true) {
                  std::function<void()> task;
                  {
                      std::unique_lock<std::mutex> lock(queueMutex);
                      condition.wait(lock, [this] {
                          return stop || !tasks.empty();
                      });

                      if (stop && tasks.empty())
                          return;

                      task = std::move(tasks.front());
                      tasks.pop();
                  }
                  task();
              } });
  }
}

ThreadPool::~ThreadPool()
{
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    stop = true;
  }

  condition.notify_all();
  for (auto &worker : workers)
    worker.join();
}

void ThreadPool::Enqueue(std::function<void()> func)
{
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    tasks.push(std::move(func));
  }
  condition.notify_one();
}