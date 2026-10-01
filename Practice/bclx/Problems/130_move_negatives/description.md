# 130 — Move All the Negative Elements to One Side of the Array

## Problem
Partition so negatives come first (two-pointer quicksort-style; order within each side not preserved).

## Input
`argv[1]`: `n`.

## Output
```
negatives first: <values>
```

## Example
```
mpirun -np 4 ./bin 130_move_negatives 16
negatives first: ...
```

## Solutions
- `v1_basic` — two-pointer partition.
