#ifndef __THREAD_H__
#define __THREAD_H__
#include <thread>
#include <utility>

class Thread {
 private:
  std::thread thread;

 public:
  Thread();
  Thread(const Thread& other) = delete;
  Thread(Thread&& other);
  void start();
  void join();
  virtual void run() = 0;
  Thread& operator=(const Thread&) = delete;
  Thread& operator=(Thread&& other) {
    this->thread = std::move(other.thread);
    return *this;
  }
  virtual ~Thread() {}
};
#endif
