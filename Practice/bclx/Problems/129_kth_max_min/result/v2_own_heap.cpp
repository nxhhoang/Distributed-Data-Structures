// 129 — Kth Max and Min Element | v2: OWN heaps, distributed over chunks
// No std::priority_queue: a hand-rolled sift-up/sift-down binary heap.
// Each rank streams its chunk keeping a size-k MAX-heap (the k smallest
// seen so far — pop discards the largest) and a size-k MIN-heap (the k
// largest). The P*k candidates are aput-ed to rank 0, which merges them
// with its own size-k heap and reads the kth element.
// Correctness: the global k smallest (largest) all sit in their OWN rank's
// heap, so they survive into the merge.
// Run: make run PROB=129_kth_max_min SRC=result/v2_own_heap.cpp NP=4 ARGS="60 3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

// ---- hand-rolled binary heaps (key = value) ----
struct MaxHeap {
    std::vector<uint64_t> a;
    void push(uint64_t v) {
        a.push_back(v);
        size_t i = a.size() - 1;
        while (i > 0) {
            size_t p = (i - 1) / 2;
            if (a[p] >= a[i]) break;
            std::swap(a[p], a[i]);
            i = p;
        }
    }
    uint64_t top() const { return a[0]; }
    void pop() {
        a[0] = a.back();
        a.pop_back();
        size_t i = 0;
        while (true) {
            size_t l = 2 * i + 1, r = l + 1, m = i;
            if (l < a.size() && a[l] > a[m]) m = l;
            if (r < a.size() && a[r] > a[m]) m = r;
            if (m == i) break;
            std::swap(a[i], a[m]);
            i = m;
        }
    }
};

struct MinHeap {
    std::vector<uint64_t> a;
    void push(uint64_t v) {
        a.push_back(v);
        size_t i = a.size() - 1;
        while (i > 0) {
            size_t p = (i - 1) / 2;
            if (a[p] <= a[i]) break;
            std::swap(a[p], a[i]);
            i = p;
        }
    }
    uint64_t top() const { return a[0]; }
    void pop() {
        a[0] = a.back();
        a.pop_back();
        size_t i = 0;
        while (true) {
            size_t l = 2 * i + 1, r = l + 1, m = i;
            if (l < a.size() && a[l] < a[m]) m = l;
            if (r < a.size() && a[r] < a[m]) m = r;
            if (m == i) break;
            std::swap(a[i], a[m]);
            i = m;
        }
    }
};

// stream my chunk: keep the k smallest (MaxHeap) and the k largest (MinHeap)
static void stream_chunk(uint64_t lo, uint64_t hi, uint64_t k,
                         MaxHeap &small, MinHeap &large) {
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t v = value_at(i);
        if (small.a.size() < k) small.push(v);
        else if (v < small.top()) { small.pop(); small.push(v); }

        if (large.a.size() < k) large.push(v);
        else if (v > large.top()) { large.pop(); large.push(v); }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <k>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0, k = 0;
    if (me == 0) {
        n = strtoull(argv[1], nullptr, 10);
        k = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    k = BCL::broadcast(k, 0);
    if (n == 0) n = 1;
    if (k == 0 || k > n) k = 1;

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;

    MaxHeap small;   // my k smallest (top = my kth smallest)
    MinHeap large;   // my k largest  (top = my kth largest)
    stream_chunk(lo, hi, k, small, large);

    // ship the candidates to rank 0: P*k slots each (junk-padded)
    BCL::GlobalPtr<uint64_t> parts_small = nullptr, parts_large = nullptr;
    if (me == 0) {
        parts_small = BCL::alloc<uint64_t>(P * k);
        parts_large = BCL::alloc<uint64_t>(P * k);
    }
    parts_small = BCL::broadcast(parts_small, 0);
    parts_large = BCL::broadcast(parts_large, 0);

    // my candidates; pad so every rank writes exactly k words
    std::vector<uint64_t> cs(small.a.begin(), small.a.end());
    std::vector<uint64_t> cl(large.a.begin(), large.a.end());
    cs.resize(k, ~0ull);   // huge junk never wins a "smallest" competition
    cl.resize(k, 0);       // zero junk never wins a "largest" competition
    for (uint64_t j = 0; j < k; ++j) {
        bclx::aput_sync(cs[j], parts_small + me * k + j);
        bclx::aput_sync(cl[j], parts_large + me * k + j);
    }

    bclx::barrier_sync();

    if (me == 0) {
        // merge: size-k heaps over the candidates
        MaxHeap ms;   // k smallest overall
        for (uint64_t r = 0; r < P; ++r)
            for (uint64_t j = 0; j < k; ++j) {
                uint64_t v = parts_small.local()[r * k + j];
                if (v == ~0ull) continue;
                if (ms.a.size() < k) ms.push(v);
                else if (v < ms.top()) { ms.pop(); ms.push(v); }
            }
        MinHeap ml;   // k largest overall
        for (uint64_t r = 0; r < P; ++r)
            for (uint64_t j = 0; j < k; ++j) {
                uint64_t v = parts_large.local()[r * k + j];
                if (ml.a.size() < k) ml.push(v);
                else if (v > ml.top()) { ml.pop(); ml.push(v); }
            }

        printf("%llu-th smallest = %llu, %llu-th largest = %llu (own heaps, %llu ranks)\n",
               k, ms.top(), k, ml.top(), P);
        BCL::dealloc<uint64_t>(parts_small);
        BCL::dealloc<uint64_t>(parts_large);
    }

    BCL::finalize();
    return 0;
}
