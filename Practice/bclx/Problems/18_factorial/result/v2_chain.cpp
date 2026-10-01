// 18 — Factorial of a Number | v2: chain reduction with aget/aput
// Each rank computes the product of its chunk of [1..n], then the partials
// are chained rank by rank: rank r WAITS (polling aget) until rank r-1's box
// is non-zero, multiplies in its own partial, and passes the running product
// on. 0 works as the "not ready" sentinel because partial products are >= 1.
// This is a reduction built by hand — and also a taste of why polling
// patterns need care in real systems.
// Run: make run PROB=18_factorial SRC=result/v2_chain.cpp NP=4 ARGS="20"

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 20) n = 20;

    // my chunk of [1..n]
    uint64_t lo = n * me / P + 1;
    uint64_t hi = n * (me + 1) / P;
    uint64_t partial = 1;
    for (uint64_t x = lo; x <= hi; ++x) partial *= x;

    // one "mailbox" word per rank; 0 = not ready yet
    std::vector<BCL::GlobalPtr<uint64_t>> box(P);
    box[me] = BCL::alloc<uint64_t>(1);
    *box[me].local() = 0;
    for (uint64_t r = 0; r < P; ++r)
        box[r] = BCL::broadcast(box[r], r);

    if (me == 0) {
        bclx::aput_sync(partial, box[0]);
    } else {
        uint64_t prev;
        while ((prev = bclx::aget_sync(box[me - 1])) == 0)
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        bclx::aput_sync(prev * partial, box[me]);
    }

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t f = bclx::aget_sync(box[P - 1]);
        printf("%llu! = %llu (chain of %llu ranks)\n", n, f, P);
    }

    BCL::dealloc<uint64_t>(box[me]);   // each rank frees its own mailbox
    BCL::finalize();
    return 0;
}
