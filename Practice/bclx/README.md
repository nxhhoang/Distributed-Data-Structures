# Practice — Mastering BCL CoreX Through 165 Classic Problems

Goal: gradually get fluent with the BCL CoreX (`bclx`) API — starting with
`broadcast` / `allreduce` on the easier problems, up to `BCL::alloc` +
`aget` / `aput` / `fao` on the ones where distribution actually makes sense —
before returning to `LoadBalancing/` (the deque + work-stealing level).

## Layout

```
Practice/bclx/
├── README.md          <- this file (hint table + primitive cheat sheet)
├── Makefile
└── Problems/
    └── NN_problem_name/
        ├── mine/      <- YOUR code (mine.cpp is a stub with BCL::init/finalize boilerplate)
        └── result/    <- reference solutions (v1_basic, v2_* — multiple ways where meaningful)
```

## Build & run (Linux / WSL, needs `mpic++`)

```bash
cd Practice/bclx

# run a reference solution:
make run PROB=03_sum_of_first_n_natural SRC=result/v2_allreduce.cpp NP=4 ARGS="1000000"

# run your own code:
make run PROB=09_prime_number SRC=mine/mine.cpp NP=2 ARGS="1000000007"

make clean   # remove the .out files
```

Manually (without make):

```bash
mpic++ -std=gnu++17 -O2 -I../../bcl -I../../bclx \
    Problems/01_positive_or_negative/result/v1_basic.cpp -o t
export OMPI_MCA_osc=pt2pt     # required with Open MPI (OMPI RMA issue)
mpirun -np 4 ./t -42
```

## Primitive cheat sheet

| What you need | Primitive | MPI underneath | When to use |
|---|---|---|---|
| Read/write MY OWN memory | `bclx::load/store` | memcpy | owner accessing its own segment |
| Remote read/write, no contention | `bclx::rget_sync` / `rput_sync` | `MPI_Get/Put` | data written by a single rank (e.g. after winning a CAS) |
| Remote read/write, CONTENDED | `bclx::aget_sync` / `aput_sync` | `Get_accumulate/Replace` | variables/counters several ranks fight over |
| Atomic `x = x op v` | `bclx::fao_sync(g, v, BCL::plus<uint64_t>{})` | `Fetch_and_op` | counting, taking an atomic append index |
| Compare-and-swap | `bclx::cas_sync(g, old, new)` | `Compare_and_swap` | lock-free retry loops |
| Sync / reduce / broadcast | `bclx::barrier_sync`, `bclx::allreduce`/`reduce`, `BCL::broadcast` | collectives | SPMD, aggregation |
| PGAS heap alloc / free | `BCL::alloc<T>(n)` / `BCL::dealloc<T>` | symmetric heap | distributed "shared variables" |

Conventions worth memorizing:
- **Ownership**: whoever calls `BCL::alloc` calls `BCL::dealloc`.
- **Sharing a gptr**: rank 0 allocates, then `BCL::broadcast(gptr, 0)` to the world.
- **fao returns the OLD value** — that is how you take an atomic append index (`idx = fao(+1)`).
- **Collectives**: `broadcast` / `allreduce` / `barrier_sync` must be called by EVERY rank, in the same order.

## The 165 problems + hints (which primitives to train, what results exist)

Group **A** = single input, local O(1)–O(sqrt(n)) work (distribution is API
practice — usually `broadcast` so every rank has the input). Group **B** =
the input is a range/array → partition + reduce/fao is the "real" approach.
Group **C** = inherently sequential.

| # | Problem | Group | Hint — primitives to train | Available results |
|---|---|---|---|---|
| 01 | Positive or Negative | A | `broadcast` | `v1_basic` |
| 02 | Even or Odd | A/B | `broadcast`; batch variant: `alloc` + `fao` counters | `v1_basic`, `v2_batch_count` |
| 03 | Sum of First N Natural | B | closed form vs partition + `allreduce` (cross-check) | `v1_formula`, `v2_allreduce` |
| 04 | Sum of N Numbers | B | array partition + `allreduce`; PGAS variant: `alloc` + bulk `rget` verify | `v1_allreduce`, `v2_pgas_verify` |
| 05 | Sum in Range [L,R] | B | `allreduce`; "manual reduce" variant: `aput` partials, rank 0 adds | `v1_allreduce`, `v2_aput_aget` |
| 06 | Greatest of Two | A | `broadcast` x2; PGAS variant: values live in rank 0's heap, everyone `aget`s | `v1_basic`, `v2_pgas` |
| 07 | Greatest of Three | A | `broadcast` x3 | `v1_basic` |
| 08 | Leap Year | A | `broadcast` | `v1_basic` |
| 09 | Prime Number | A/B | v1 trial division; v2 stripes sqrt(n) candidates over ranks + `allreduce` | `v1_basic`, `v2_parallel` |
| 10 | Prime in Range | B | partition + print per rank; collect variant: `fao` index + `aput` | `v1_print`, `v2_collect` |
| 11 | Sum of Digits | A | `broadcast` | `v1_basic` |
| 12 | Reverse a Number | A | `broadcast` | `v1_basic` |
| 13 | Palindrome Number | A | `broadcast` | `v1_basic` |
| 14 | Armstrong Number | A | `broadcast` | `v1_basic` |
| 15 | Armstrong in Range | B | `fao` + `aput` result collection (like 10/v2) | `v2_collect` |
| 16 | Fibonacci Series | C | sequential + DISTRIBUTED storage: term i at rank `i%P`, slot `i/P`, `aput` + verify | `v1_distributed_store` |
| 17 | Nth Fibonacci | C | iterative; fast-doubling O(log n) variant cross-checked across 2 ranks via `aput`/`aget` | `v1_iterative`, `v2_fast_doubling` |
| 18 | Factorial | B | v1 basic; v2 chain: `aget` previous rank's partial -> multiply -> `aput` to next | `v1_basic`, `v2_chain` |
| 19 | Power of a Number | A | fast exponentiation (square-and-multiply) on rank 0 | `v1_fast_pow` |
| 20 | Factor of a Number | B | partition [1..n], each rank prints its found divisors | `v1_print` |
| 21 | Prime Factors | A/B | v1 trial division; v2 finds prime divisors in parallel (`fao` + `aput`), rank 0 computes multiplicities | `v1_basic`, `v2_parallel` |
| 22 | Strong Number | A | `broadcast` (factorial table for digits 0..9) | `v1_basic` |
| 23 | Perfect Number | A | `broadcast`, paired-divisor loop i*i<=n | `v1_basic` |
| 24 | Perfect Square | A | `broadcast`, sqrtl + rounding fix | `v1_basic` |
| 25 | Automorphic Number | A | `broadcast`, square in `unsigned __int128` | `v1_basic` |
| 26 | Harshad Number | A | `broadcast` | `v1_basic` |
| 27 | Abundant Number | A | `broadcast`, proper divisor sum > n | `v1_basic` |
| 28 | Friendly Pair | A/B | v1 basic (sigma(a)/a == sigma(b)/b, cross-multiplied with `__int128`); v2: two ranks compute one sigma each | `v1_basic`, `v2_two_ranks` |
| 29 | HCF | A | `broadcast`, iterative Euclid | `v1_basic` |
| 30 | LCM | A | `broadcast`, lcm = a/gcd*b | `v1_basic` |
| 31 | GCD | A | RECURSIVE Euclid (HCF == GCD) | `v1_recursive` |
| 32 | Binary -> Decimal | A/B | strings must ride a fixed-buffer struct to be broadcastable; v2: striped bit positions + `allreduce` | `v1_basic`, `v2_parallel_positions` |
| 33 | Octal -> Decimal | A | like 32, base 8 (Horner) | `v1_basic` |
| 34 | Hex -> Decimal | A | like 32, base 16, parse a-f/A-F | `v1_basic` |
| 35 | Decimal -> Binary | A | repeated division, print reversed | `v1_basic` |
| 36 | Decimal -> Octal | A | repeated division, print reversed | `v1_basic` |
| 37 | Decimal -> Hex | A | digit table `0123456789ABCDEF` | `v1_basic` |
| 38 | Binary -> Octal | A | left-pad to a multiple of 3, group bits | `v1_basic` |
| 39 | Octal -> Binary | A | each digit -> 3 bits, strip leading zeros | `v1_basic` |
| 40 | Quadrants | A | `broadcast` coordinates, sign + axis check | `v1_basic` |
| 41 | Permutations P(n,r) | B | v1 falling product; v2: striped factors + chained partial products (like 18/v2) | `v1_basic`, `v2_chain` |
| 42 | Max Handshakes | A | C(n,2) = n(n-1)/2 | `v1_basic` |
| 43 | Addition of Fractions | A | `broadcast` 4 values, common denominator + gcd reduction | `v1_basic` |
| 44 | Replace 0's with 1's | A | rebuild the number by place value | `v1_basic` |
| 45 | Sum of Two Primes | B | v1 rank 0 scan; v2: striped p + `fao` "first reporter wins" | `v1_basic`, `v2_parallel` |
| 46 | Count Decodings | C | Fibonacci-shaped DP — sequential (group C) | `v1_dp` |
| 47 | Area of Circle | A | `broadcast` a double, pi*r^2 | `v1_basic` |
| 48 | Primes 1..100 | B | STRIPING (x == me mod P) — contrast with the contiguous chunks of 10/v1 | `v1_striped` |
| 49 | Count Digits | A | `broadcast` + digit loop | `v1_basic` |
| 50 | Number to Words | A | English word tables, groups of three digits (billion/million/thousand) | `v1_basic` |
| 51 | Days in Month | A | `broadcast`, February honors the leap rule | `v1_basic` |
| 52 | Digit Occurrences | A/B | v1 counts within one number; v2 counts across a whole range [L..R] with `fao` | `v1_basic`, `v2_batch_range` |
| 53 | Exactly x Divisors | B | v1 rank 0; v2: partition [1..n] + `allreduce` | `v1_basic`, `v2_allreduce` |
| 54 | Quadratic Roots | A | `broadcast` 3 doubles, discriminant, complex roots included | `v1_basic` |
| 55 | Power of a Number (recursion) | A | linear recursion; v2: divide-and-conquer halving | `v1_basic`, `v2_divide_conquer` |
| 56 | Prime Number (recursion) | A | recursive divisor check `is_prime(n, d)` | `v1_basic` |
| 57 | Largest Element in an Array | B | v1 recursion on rank 0; v2: PGAS chunks + recursive max per chunk + `aput` partial maxima | `v1_basic`, `v2_allreduce` |
| 58 | Smallest Element in an Array | A | mirror of 57/v1 | `v1_basic` |
| 59 | Reverse a Number (recursion) | A | recursion with an accumulator | `v1_basic` |
| 60 | HCF Using Recursion | A | recursive Euclid (same as 31/v1) | `v1_basic` |
| 61 | LCM (recursive gcd) | A | lcm built on the recursive gcd | `v1_basic` |
| 62 | String Length Using Recursion | A | `len(s) = 1 + len(s+1)` on the broadcast struct | `v1_basic` |
| 63 | All Permutations of a String | B/C | v1 swap recursion; v2: STRIPE the top-level branches of the recursion tree over ranks + `allreduce` count check | `v1_basic`, `v2_striped_branches` |
| 64 | F(N)th Term (recursive sequence) | C | t(n) = t(n-1)^2 + t(n-2)^2, plain recursion (n <= 8: overflow) | `v1_basic` |
| 65 | Sums of All Subsets | B | v1 include/exclude recursion; v2: stripe the top-level 2^m prefixes (2^m >= P) + `fao` count check | `v1_basic`, `v2_striped_prefixes` |
| 66 | Last Non-Zero Digit of n! | A | the classic D(n) recursion with the {1,1,2,6,4,2,2,4,2,8} table | `v1_basic` |
| 67 | Nth Row of Pascal's Triangle | B | v1 recursive binomial; v2: striped k + `aput` row collection | `v1_basic`, `v2_striped` |
| 68 | Balanced Parentheses | C | backtracking recursion with open/close counters | `v1_basic` |
| 69 | Factorial Using Recursion | A | plain recursion (see 18/v2 for the chain variant) | `v1_basic` |
| 70 | Palindromic Partitions | C | backtracking over cut positions + palindrome check | `v1_basic` |
| 71 | N-bit Binaries, 1s >= 0s | C | backtracking with running counters | `v1_basic` |
| 72 | All Subsets of a Set | B | include/exclude recursion (65/v2 shows the parallel variant) | `v1_basic` |
| 73 | Remove Adjacent Duplicates Recursively | C | run-collapse recursion | `v1_basic` |
| 74 | Largest Element in an Array | A/B | loop scan (57/v2 has the distributed variant) | `v1_basic` |
| 75 | Smallest Element in an Array | A | loop scan | `v1_basic` |
| 76 | Smallest AND Largest Element | B | one pass with both trackers; v2: chunk min/max + `aput` pairs | `v1_basic`, `v2_aput_minmax` |
| 77 | Second Smallest Element | A | one pass, two trackers, distinct values | `v1_basic` |
| 78 | Sum of Elements in an Array | B | loop (04 has the partition + `allreduce` version) | `v1_basic` |
| 79 | Reverse an Array | B | two pointers; v2: distributed mirrored swap via `aget` + barrier | `v1_basic`, `v2_pgas_swap` |
| 80 | Sort Half Asc / Half Desc | A | two `std::sort` calls | `v1_basic` |
| 81 | Sort the Elements of an Array | A | `std::sort` | `v1_basic` |
| 82 | Frequency of Elements | B | map counting; v2: LOCAL histogram per chunk + batched `fao` into rank 0's table | `v1_basic`, `v2_pgas_histogram` |
| 83 | Sort Elements by Frequency | A | freq map + stable sort (freq desc, value asc) | `v1_basic` |
| 84 | Longest Palindrome in an Array | B | per-element check; v2: chunk best + `aput` (digits, value) pairs | `v1_basic`, `v2_aput_best` |
| 85 | Count Distinct Elements | A | set | `v1_basic` |
| 86 | Repeating Elements | A | freq map, count > 1 | `v1_basic` |
| 87 | Non-Repeating Elements | A | freq map, count == 1 | `v1_basic` |
| 88 | Remove Duplicates (order kept) | A | seen-set | `v1_basic` |
| 89 | Minimum Scalar Product | B | sort a asc + b desc + dot; v2: rank 0 ships sorted chunks with bulk `rput`, local dot + `allreduce` | `v1_basic`, `v2_distributed_dot` |
| 90 | Maximum Scalar Product | A | both ascending + dot (mirror of 89/v1) | `v1_basic` |
| 91 | Count Even and Odd Elements | B | loop; v2: chunk counts + `fao` (pattern of 02/v2 on array data) | `v1_basic`, `v2_fao_counters` |
| 92 | Symmetric Pairs | A | (a,b) with (b,a) present | `v1_basic` |
| 93 | Maximum Product Sub-array | C | Kadane with max/min swap on negatives | `v1_basic` |
| 94 | Arrays Disjoint or Not | A | set intersection | `v1_basic` |
| 95 | Array Subset of Another | A | set containment | `v1_basic` |
| 96 | Can All Numbers Be Made Equal | A | strip factors 2 and 3, compare | `v1_basic` |
| 97 | Minimum Sum of Absolute Difference | A | sort + pair adjacent elements | `v1_basic` |
| 98 | Sort by Another Array's Order | A | position map + stable sort, leftovers appended | `v1_basic` |
| 99 | Replace Elements by Rank | A | sort + index map, ties share a rank | `v1_basic` |
| 100 | Equilibrium Index | A | prefix/suffix sums | `v1_basic` |
| 101 | Left and Right Rotation | A | index arithmetic, both directions | `v1_basic` |
| 102 | Block Swap Rotation | C | recursive block-swap | `v1_basic` |
| 103 | Juggling Rotation | C | gcd cycles | `v1_basic` |
| 104 | Circular Rotation by K | A | brute-force rotation search | `v1_basic` |
| 105 | Balanced Parenthesis (stack) | A | stack check (68 generates combinations) | `v1_basic` |
| 106 | Vowel or Consonant | A | `broadcast` a single char | `v1_basic` |
| 107 | Alphabet or Not | A | `broadcast` a single char | `v1_basic` |
| 108 | ASCII Value of a Character | A | `broadcast` a single char | `v1_basic` |
| 109 | String Length Without strlen() | A | loop until '\0' (62 is the recursive twin) | `v1_basic` |
| 110 | Toggle Each Character | A | case flip in place | `v1_basic` |
| 111 | Count the Number of Vowels | B | loop; v2: striped positions + `allreduce` | `v1_basic`, `v2_striped` |
| 112 | Remove the Vowels | A | filter | `v1_basic` |
| 113 | Palindrome String | B | two pointers; v2: distributed mirror compare via `aget` | `v1_basic`, `v2_pgas_mirror` |
| 114 | Print String in Reverse Order | B | backwards print; v2: PGAS mirrored swap of a char array | `v1_basic`, `v2_pgas_swap` |
| 115 | Keep Alphabets Only | A | isalpha filter | `v1_basic` |
| 116 | Remove Spaces | A | filter | `v1_basic` |
| 117 | Remove Brackets | A | filter ()[]{} from an expression | `v1_basic` |
| 118 | Sum of Numbers in a String | A | multi-digit run parsing | `v1_basic` |
| 119 | Capitalize First/Last of Each Word | A | per-word toupper | `v1_basic` |
| 120 | Frequency of Characters | B | 26-bucket table; v2: local histogram + batched `fao` (82/v2 on letters) | `v1_basic`, `v2_pgas_histogram` |
| 121 | Non-Repeating Characters | A | freq table, count == 1 | `v1_basic` |
| 122 | Anagram Check | B | histogram compare; v2: rank 0 / rank 1 count one string each | `v1_basic`, `v2_two_ranks` |
| 123 | Replace a Sub-string | A | find + rebuild | `v1_basic` |
| 124 | Replace a Word | A | space-tokenized replace | `v1_basic` |
| 125 | Count Common Sub-Sequences | C | DP with inclusion-exclusion | `v1_basic` |
| 126 | Wildcard Pattern Match | C | DP over '?' and '*' | `v1_basic` |
| 127 | Permutations in Sorted Order | C | sort + used[] backtracking, duplicates skipped (63/v2 is the striped variant) | `v1_basic` |
| 128 | Sort an Array of 0s, 1s and 2s | B | Dutch National Flag; v2: `fao` histogram + rebuild | `v1_basic`, `v2_fao_count` |
| 129 | Kth Max and Min Element | A | sort + index arithmetic | `v1_basic` |
| 130 | Move Negatives to One Side | A | two-pointer partition | `v1_basic` |
| 131 | Union and Intersection of Two Sorted Arrays | B | merge walk; v2: striped binary search + `fao` count | `v1_basic`, `v2_striped_bsearch` |
| 132 | Largest Sum Contiguous Subarray | A | O(n^2) all subarrays — the pre-Kadane view | `v1_basic` |
| 133 | Minimize Max Difference Between Heights | A | sort + boundary candidates | `v1_basic` |
| 134 | Minimum Jumps to Reach the End | C | greedy reach | `v1_basic` |
| 135 | Find Duplicate in N+1 Integers | C | Floyd's cycle detection | `v1_basic` |
| 136 | Merge 2 Sorted Arrays Without Extra Space | C | gap (shell) method | `v1_basic` |
| 137 | Kadane's Algorithm | A | O(n) DP — compare with 132 | `v1_basic` |
| 138 | Merge Intervals | A | sort by start + merge | `v1_basic` |
| 139 | Count Inversions | C | merge sort counting | `v1_basic` |
| 140 | Best Time to Buy and Sell Stock (once) | A | one pass with a running minimum | `v1_basic` |
| 141 | Pairs With a Given Sum | B | set; v2: striped outer loop + `fao` count | `v1_basic`, `v2_striped` |
| 142 | Subarray With Sum Zero | C | prefix sums + set | `v1_basic` |
| 143 | Factorial of a Large Number | C | digit-array multiplication | `v1_basic` |
| 144 | Common Elements in 3 Sorted Arrays | A | three-pointer walk | `v1_basic` |
| 145 | Alternating Positive/Negative, O(1) Space | C | right-rotation insertion | `v1_basic` |
| 146 | Elements Appearing More Than n/k Times | A | sort + run counting | `v1_basic` |
| 147 | Buy and Sell a Share At Most Twice | C | forward + backward profit passes | `v1_basic` |
| 148 | Next Permutation | C | pivot + swap + reverse (the engine of 127) | `v1_basic` |
| 149 | Longest Consecutive Subsequence | A | set + run-start check | `v1_basic` |
| 150 | Trapping Rain Water | C | two pointers | `v1_basic` |
| 151 | Chocolate Distribution | A | sort + sliding window of m | `v1_basic` |
| 152 | Smallest Subarray With Sum > X | C | sliding window | `v1_basic` |
| 153 | Three-Way Partitioning Around a Range | A | DNF variant over [a, b] | `v1_basic` |
| 154 | Min Operations to Make Array a Palindrome | C | two-pointer merging | `v1_basic` |
| 155 | Median of 2 Sorted Arrays (Equal Size) | C | partition binary search | `v1_basic` |
| 156 | Median of 2 Sorted Arrays (Different Size) | C | generalized partition search | `v1_basic` |
| 157 | Spiral Traversal on a Matrix | B | boundary walk; v2: stripe the RINGS over ranks + `allreduce` count check | `v1_basic`, `v2_striped_rings` |
| 158 | Search an Element in a Matrix | B | staircase walk; v2: striped rows + per-row bsearch, `fao` first-reporter | `v1_basic`, `v2_striped_rows` |
| 159 | Median in a Row-Wise Sorted Matrix | C | binary search over the value range | `v1_basic` |
| 160 | Row With Maximum Number of 1's | B | per-row first-1 bsearch; v2: striped rows + `aput` (count, row) pairs | `v1_basic`, `v2_aput_rows` |
| 161 | Print Sorted From Row-Column Sorted Matrix | C | k-way merge of the sorted rows | `v1_basic` |
| 162 | Find a Specific Pair in a Matrix | C | DP with the bottom-right max table | `v1_basic` |
| 163 | Rotate a Matrix by 90 Degrees | B | transpose + reverse rows; v2: PGAS scatter-write of the bijective index map | `v1_basic`, `v2_pgas_scatter` |
| 164 | Kth Smallest in a Row-Column Sorted Matrix | C | value-range binary search + counting | `v1_basic` |
| 165 | Common Elements in All Rows | A | running intersection by merge walk | `v1_basic` |

## Data-structure upgrade paths (v2/v3 extensions)

The v1 solutions mostly use local containers. These extensions move the SAME
problems onto the repo's actual distributed data structures:

**Path 1 — BCL::HashMap (the distributed hash table)** — 13 problems swap
`std::set/map` for the library's HashMap. Core patterns:
- *race-free counting*: local count -> `insert_or_assign(v, 0)` pre-seed ->
  barrier -> `modify(v, +count)` (atomic RMW on existing entries; never
  modify a fresh slot — it reads uninitialized memory).
- *membership*: striped inserts, barrier, striped `find` probes.

| Problem | File | Pattern |
|---|---|---|
| 82 frequency | `v3_hashmap` | count + local-segment iteration + total check |
| 85 distinct | `v2_hashmap` | striped inserts, count occupied slots per segment |
| 86 repeating / 87 non-repeating | `v2_hashmap` | count, report per segment |
| 88 remove duplicates (order kept) | `v2_hashmap` | value -> first index (min-merge), sort by index |
| 94 disjoint | `v2_hashmap` | membership + fao first-reporter |
| 95 subset | `v2_hashmap` | containment + fao first-miss |
| 122 anagram | `v3_hashmap` | +1/-1 letter counting, all zeros |
| 131 union/intersection | `v3_hashmap` | membership + join-on-miss + n+m-inter check |
| 141 pairs with sum | `v3_hashmap` | one-pass seen-map, rank 0 drives |
| 142 zero-sum subarray | `v2_hashmap` | prefix-sum markers, rank 0 drives |
| 146 more than n/k | `v2_hashmap` | count + threshold report |
| 149 longest consecutive | `v2_hashmap` | run-start probing + aput composite reduce |

**Path 2 — the dds nonblocking stack** — 105/114 use `stack/inc/stack_treiber.h`
(the distributed Treiber stack with the NMR memory manager; push/pop are
one-sided RMA). NOTE: the dds memory managers depend on a missing `sds`
module from the original repo — restored as `memory/inc/sds.h`.
- 105 `v2_distributed_stack`: rank 0 drives parenthesis matching on the global stack.
- 114 `v2_distributed_stack`: phased pushes (rank order) then phased pops
  (reverse rank order) — LIFO makes each rank pop exactly its own chunk
  reversed; print order = the reversed string.

**Path 3 — BCL::FastQueue and own heaps**
- 68/71 `v2_bfs_fastqueue`: level-synchronized BFS on
  `BCL::FastQueue<std::string>` (strings ride the ObjectContainer
  serialization): barrier -> `size()` -> pop my slice -> push children.
  Catalan / DFS cross-checks.
- 129 `v2_own_heap`: hand-rolled size-k heaps per chunk + aput candidate
  merge (no std::priority_queue).
- 161 `v2_own_heap`: own min-heap k-way merge of the sorted rows.
- 164 `v2_own_heap`: own heap over row heads vs v1's value binary search.

## Suggested workflow for each problem

1. Read the hint in the table above -> **write it yourself in `mine/mine.cpp`**
   (before looking at the result!).
2. Run with `NP=1`, then `NP=4` — the output must be identical.
3. Open `result/` to compare and ask yourself: *why did the solution pick this
   primitive?*
4. For group B problems: try changing the chunking (contiguous vs striped) and
   watch the load balance (hint: `is_prime(x)` gets costlier as x grows, so the
   last chunk is heavier — connect that to work stealing).

## After this set

You will have gone through `broadcast` -> `allreduce` -> `alloc`/`aget`/`aput`
-> `fao` -> distributed arrays -> aget/aput chains. The next step is
**`LoadBalancing/`** in the repo: the same primitives, but at the level of
ABP / steal-half deques — where `cas` and 64-bit packing really start to
matter.
