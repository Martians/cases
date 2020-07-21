

#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include "Common.h"

template<typename F>
class ScopedCleanup {
public:
    explicit ScopedCleanup(F f)
            : cancelled_(false),
              f_(std::move(f)) {
    }
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
template<typename F>
ScopedCleanup<F> MakeScopedCleanup(F f) {
    return ScopedCleanup<F>(f);
}


void
test() {
    auto a = MakeScopedCleanup( []{ printf("%d", 1);});
    printf("%d", 2);

}


int main(int argc, char* argv[]) {
    // test();
    common::Test();
    return 0;
}