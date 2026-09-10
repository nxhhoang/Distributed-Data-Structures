// 06 — Greatest of Two Numbers | v2: values live in the PGAS heap
// The two numbers are stored in rank 0's PGAS heap; every rank reads them
// REMOTELY with bclx::aget_sync and computes the max itself. This is the
// "shared variable" flavor of PGAS: memory belongs to a rank but is
// readable by everyone.
// Run: make run PROB=06_greatest_of_two SRC=result/v2_pgas.cpp NP=4 ARGS="17 42"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t me = BCL::rank();

    BCL::GlobalPtr<uint64_t> g = nullptr;
    if (me == 0) {
        g = BCL::alloc<uint64_t>(2);
        g.local()[0] = strtoull(argv[1], nullptr, 10);
        g.local()[1] = strtoull(argv[2], nullptr, 10);
    }
    g = BCL::broadcast(g, 0);

    // every rank reads the same PGAS words (remotely for r != 0)
    uint64_t a = bclx::aget_sync(g + 0);
    uint64_t b = bclx::aget_sync(g + 1);
    uint64_t m = a > b ? a : b;

    if (me == 0) {
        printf("greatest(%llu, %llu) = %llu (all ranks computed this)\n", a, b, m);
        BCL::dealloc<uint64_t>(g);
    }

    BCL::finalize();
    return 0;
}
