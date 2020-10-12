

#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "Common.h"
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

template <typename F> class ScopedCleanup {
public:
  explicit ScopedCleanup(F f) : cancelled_(false), f_(std::move(f)) {}
  ~ScopedCleanup() {
    if (!cancelled_) {
      f_();
    }
  }
  void cancel() { cancelled_ = true; }

private:
  bool cancelled_;
  F f_;
};
// Creates a new scoped cleanup instance with the provided function.
template <typename F> ScopedCleanup<F> MakeScopedCleanup(F f) {
  return ScopedCleanup<F>(f);
}

void test() {
  auto a = MakeScopedCleanup([] { printf("%d", 1); });
  printf("%d", 2);
}

void MapTest() {
  std::map<int, int> map;
  map[1] = 1;
  auto a = map[2];
  std::cout << a << std::endl;
}


int actual(int length) {
  int origin = length;
  int base = 1;
  while(length > 1) {
    length >>= 1;
    base <<= 1;
  }
  if (origin > base) {
    base <<= 1;
  }
  std::cout << "origin: " << origin << ", to: " << base << std::endl;
  return base;
}

int main(int argc, char *argv[]) {
  common::Test();
  return 0;
}