#ifndef PERIODIC_EXECUTOR_H
#define PERIODIC_EXECUTOR_H

#include <future>
#include <thread>
#include <chrono>

namespace Patrick {

  template<class T>
  struct PeriodicExecutor
  {
    PeriodicExecutor(int timer, T& obj, std::function<void(T&)> f) {
      start(timer, obj, f);
    }

    void start(int timer, T& obj, std::function<void(T&)> f) {
      while(true) {
        auto result = std::async(std::launch::async, [timer, &obj, &f] {
            std::this_thread::sleep_for(std::chrono::seconds( timer ));
            f(obj);
        });
      }
    }
  };
}
#endif
