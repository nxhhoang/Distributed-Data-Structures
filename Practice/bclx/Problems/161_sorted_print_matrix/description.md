# 161 — Print Elements in Sorted Order Using a Row-Column Sorted Matrix

## Problem
Merge R sorted rows in sorted order.

## Input
`argv[1]`: `R`, `argv[2]`: `C`.

## Output
```
sorted: <values>
```

## Example
```
mpirun -np 4 ./bin 161_sorted_print_matrix 4 5
sorted: 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39
```

## Solutions
- `v1_basic` — k-way merge (linear head scan).
- `v2_own_heap` — hand-rolled min-heap k-way merge.
