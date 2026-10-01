# 165 — Find Common Elements in All Rows of a Given Matrix

## Problem
Running intersection: start with row 0, merge-walk with each subsequent row.

## Input
`argv[1]`: `R`, `argv[2]`: `C` (every row contains {7, 21, 35} + row-specific values).

## Output
```
common elements in all <R> rows: <values>
```

## Example
```
mpirun -np 4 ./bin 165_common_all_rows 5 8
common elements in all 5 rows: 7 21 35
```

## Solutions
- `v1_basic` — running intersection by merge walk.
