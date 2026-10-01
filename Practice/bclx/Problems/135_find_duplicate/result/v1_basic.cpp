// 135 — Find the Duplicate in an Array of N+1 Integers | v1: Floyd's cycle detection
// Values are in [1..n] with n+1 slots, so the array is a function i -> a[i]
// whose "rho" shape guarantees a cycle entered from the duplicate.
// The generated array has n+1 elements in 1..n — value 1 appears twice.
// Run: make run PROB=135_find_duplicate SRC=result/v1_basic.cpp NP=4 ARGS="10"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i, uint64_t n) {
    return 1 + (i % n);   // n+1 values in 1..n -> exactly one duplicate
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n (array has n+1 elements in 1..n)>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n + 1);
        for (uint64_t i = 0; i <= n; ++i) a[i] = value_at(i, n);

        // Floyd's tortoise and hare on the implicit linked list i -> a[i]
        uint64_t slow = a[0], fast = a[a[0]];
        while (slow != fast) {
            slow = a[slow];
            fast = a[a[fast]];
        }
        slow = 0;                       // restart; meeting point finds the entry
        while (slow != fast) {
            slow = a[slow];
            fast = a[fast];
        }

        printf("duplicate element = %llu (Floyd's cycle)\n", slow);
    }

    BCL::finalize();
    return 0;
}
