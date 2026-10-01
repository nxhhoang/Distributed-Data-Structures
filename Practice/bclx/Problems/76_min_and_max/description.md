# 76 — Find the Smallest and Largest Element in an Array

## Problem
Find both extrema in one pass.

## Input
`argv[1]`: `n`.

## Output
```
smallest = <min>, largest = <max>
```

## Example
```
mpirun -np 4 ./bin 76_min_and_max 16
smallest = 0, largest = 88 (one pass over 16 elements)
```

## Solutions
- `v1_basic` — one pass with both trackers.
- `v2_aput_minmax` — PGAS chunks + per-rank (min, max) pairs via `aput` into rank 0's slots; rank 0 combines.
