# 164 — Kth Smallest in a Row-Column Sorted Matrix

## Problem
Two approaches: value-range binary search + counting (v1), or min-heap over row heads (v2).

## Input
`argv[1]`: `R`, `argv[2]`: `C`, `argv[3]`: `k`.

## Output
```
<k>-th smallest element = <value>
```

## Example
```
mpirun -np 4 ./bin 164_kth_smallest_matrix 4 5 8
8-th smallest element = 15
```

## Solutions
- `v1_basic` — value-range binary search + upper_bound.
- `v2_own_heap` — heap over row heads.
