# 155 — Median of 2 Sorted Arrays of Equal Size

## Problem
Partition search: find i elements from A, n-i from B so all left <= all right; median = avg of boundary max/min.

## Input
`argv[1]`: `n A`, `argv[2]`: `n B` (equal sizes).

## Output
```
median of the two sorted arrays = <value>
```

## Example
```
mpirun -np 4 ./bin 155_median_two_equal 6 6
median of the two sorted arrays = 8.0
```

## Solutions
- `v1_basic` — partition binary search.
