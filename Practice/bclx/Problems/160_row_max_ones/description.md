# 160 — Find Row With Maximum Number of 1's

## Problem
Rows sorted (0s then 1s); count = C - index of first 1.

## Input
`argv[1]`: `R`, `argv[2]`: `C`.

## Output
```
row <r> has the most 1's: <count> of <C>
```

## Example
```
mpirun -np 4 ./bin 160_row_max_ones 6 8
row 5 has the most 1's: 8 of 8
```

## Solutions
- `v1_basic` — per-row first-1 binary search.
- `v2_aput_rows` — striped rows + `aput` (count, row) pairs.
