# 03 — Sum of First N Natural Numbers

## Problem
Compute 1 + 2 + ... + N.

## Input
`argv[1]`: `N` (fits uint64; N*(N+1) must fit).

## Output
```
1 + 2 + ... + <N> = <sum>
```

## Example
```
mpirun -np 4 ./bin 03_sum_of_first_n_natural 100
1 + 2 + ... + 100 = 5050
```

## Solutions
- `v1_formula` — closed form N*(N+1)/2 on rank 0.
- `v2_allreduce` — partition [1..N] into contiguous chunks, each rank sums its chunk, combine with `bclx::allreduce`. Cross-checks against the formula (prints MATCH).
