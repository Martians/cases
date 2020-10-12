
#include <iostream>
#include <jemalloc/jemalloc.h>

void
do_something(size_t i) {
    // Leak some memory.
    void* a = malloc(i * 100);
    std::cout << je_malloc_usable_size(a) << std::endl;
}

int
main(int argc, char* argv[]) {
    for (size_t i = 0; i < 1000; i++) {
        do_something(i);
    }
    je_malloc_stats_print(NULL, NULL, NULL);
    size_t epoch = 1, allocated = 0, resident = 0, active = 0, mapped = 0, sz = sizeof(size_t);
    je_mallctl("stats.mapped", &mapped, &sz, NULL, 0);
    return 0;
}