#ifndef PERIODIC_EXECUTOR_H
#define PERIODIC_EXECUTOR_H

#include <future>
#include <thread>
#include <chrono>

namespace Patrick {

  template<class T>
  struct PeriodicExecutor
  {
    PeriodicExecutor(int timer, std::shared_ptr<T>& obj, std::function<void(T&)> f) {
      start(timer, obj, f);
    }

    void start(int timer, std::shared_ptr<T>& obj, std::function<void(T&)> f) {
      std::thread t([=] {
        while(true) {
          std::this_thread::sleep_for(std::chrono::seconds( timer ));
          f(*obj);
        }
      });
      t.detach();
    }
  };
}
#endif
