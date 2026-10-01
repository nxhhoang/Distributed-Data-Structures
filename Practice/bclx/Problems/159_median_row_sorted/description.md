# 159 — Find Median in a Row-Wise Sorted Matrix

## Problem
Binary search over the VALUE range: count elements <= mid via `upper_bound` per row.

## Input
`argv[1]`: `R`, `argv[2]`: `C` (R*C is odd).

## Output
```
median of the <R> x <C> row-sorted matrix = <value>
```

## Example
```
mpirun -np 4 ./bin 159_median_row_sorted 3 5
median of the 3 x 5 row-sorted matrix = 539
```

## Solutions
- `v1_basic` — value-range binary search.
