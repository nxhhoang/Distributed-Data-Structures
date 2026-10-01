# 144 — Find Common Elements in 3 Sorted Arrays

## Problem
Three-pointer walk on three sorted arrays.

## Input
`argv[1..3]`: `n A`, `m B`, `k C`.

## Output
```
common elements: <values>
```

## Example
```
mpirun -np 4 ./bin 144_common_three_sorted 10 12 15
common elements: 0 12 24 36
```

## Solutions
- `v1_basic` — three-pointer walk.
