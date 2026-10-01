# 128 — Sort an Array of 0s, 1s and 2s

## Problem
Sort an array containing only {0, 1, 2} in one pass using the Dutch National Flag algorithm.

## Input
`argv[1]`: `n` (values generated as `(i*k+17)%3`).

## Output
```
sorted: <values>  (v2: plus "0s=<c0> 1s=<c1> 2s=<c2>, total <n> -> MATCH")
```

## Example
```
mpirun -np 4 ./bin 128_sort_012_dnf 16
sorted: 0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 2
```

## Solutions
- `v1_basic` — DNF three-region one pass.
- `v2_fao_count` — fao histogram of 3 buckets + rebuild.
