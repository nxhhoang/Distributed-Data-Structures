# 88 — Removing Duplicate Elements from an Array

## Problem
Remove duplicates, preserving first-occurrence order.

## Input
`argv[1]`: `n`.

## Output
```
duplicates removed (order kept): <values>
```

## Example
```
mpirun -np 4 ./bin 88_remove_duplicates 40
duplicates removed (order kept): ...
```

## Solutions
- `v1_basic` — seen-set.
- `v2_hashmap` — value -> first index (min-merge with `modify`), sort by index.
