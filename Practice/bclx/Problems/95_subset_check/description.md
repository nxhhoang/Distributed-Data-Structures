# 95 — Determine Whether an Array Is a Subset of Another

## Problem
Check if every element of B exists in A.

## Input
`argv[1]`: `n A`, `argv[2]`: `m B` (B is drawn from A).

## Output
```
B is a subset of A  |  B is NOT a subset of A (missing element: <v>)
```

## Example
```
mpirun -np 4 ./bin 95_subset_check 20 8
B is a subset of A
```

## Solutions
- `v1_basic` — set containment.
- `v2_hashmap` — striped inserts + find probes + fao first-miss.
