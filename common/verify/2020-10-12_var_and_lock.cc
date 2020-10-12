#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <assert.h>

bool done = false;
std::mutex cv_m;
std::condition_variable cv;
using namespace std::chrono_literals;

void Wait() {
  {
    std::unique_lock<std::mutex> lk(cv_m);
    std::cout << "work wait" << std::endl;
    cv.wait(lk, [] { return done; });
    std::cout << "work wakeup" << std::endl;
  }

  {
    std::unique_lock<std::mutex> lk(cv_m);
    std::cout << "work wait again" << std::endl;
    cv.wait_until(lk, std::chrono::system_clock::now() + 1s, [] { return false; });
    std::cout << "work wakeup again" << std::endl;
  }
}

void Notify() {
  std::cout << "notify" << std::endl;
  cv.notify_all();
  std::cout << "notify done" << std::endl;
}

// struct Test {
//   std::thread th_;
//   bool stop = false;
//
//   Test() {
//     Backend();
//   }
//   ~Test() {
//     stop = true;
//     th_.join();
//   }
//   void Backend() {
//     th_ = std::thread([&] {
//       std::this_thread::sleep_for(std::chrono::milliseconds (1000));
//       while (!stop) {
//         std::this_thread::sleep_for(std::chrono::milliseconds (1000));
//         Notify();
//       }
//     });
//   }
// };
// Test s_test;

int main(int argc, char *argv[]) {
  // normal
  std::thread t1(Wait);
  std::this_thread::sleep_for(std::chrono::milliseconds (100));
  {
    std::unique_lock<std::mutex> lk(cv_m);
    done = true;
  }
  Notify();

  if (true) {
    // abnormal
    std::this_thread::sleep_for(std::chrono::milliseconds (300));
    std::cout << "destroy" << std::endl;
    // assert(pthread_mutex_destroy(cv_m.native_handle()) == 0);
    t1.join();

    assert(pthread_cond_destroy(cv.native_handle()) == 0);
    Notify();
  }

  std::cout << "exit" << std::endl;
  return 0;
}