# 80 — Sort First Half Ascending and Second Half Descending

## Problem
Sort the first N/2 elements ascending and the last N/2 descending (each half sorted independently).

## Input
`argv[1]`: `n`.

## Output
```
sorted (<n>/2 asc + desc): <values>
```

## Example
```
mpirun -np 4 ./bin 80_sort_half_asc_desc 10
sorted (10/2 asc + desc): 0 17 39 61 78 83 66 44 22 5
```

## Solutions
- `v1_basic` — two `std::sort` calls (first asc, second desc).
