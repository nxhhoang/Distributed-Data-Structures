# 139 — Count Inversion

## Problem
Count pairs (i < j) with a[i] > a[j], via merge sort counting.

## Input
`argv[1]`: `n`.

## Output
```
inversions = <count> (merge sort count)
```

## Example
```
mpirun -np 4 ./bin 139_count_inversions 12
inversions = 26 (merge sort count)
```

## Solutions
- `v1_basic` — merge sort counting.
