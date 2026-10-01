# 79 — Reverse an Array

## Problem
Reverse the element order of an array.

## Input
`argv[1]`: `n`.

## Output
```
original: <v0> <v1> ... 
reversed: <vn-1> ... <v0>
```

## Example
```
mpirun -np 4 ./bin 79_reverse_array 10
original: <first 10 values>
reversed: <same values reversed>
```

## Solutions
- `v1_basic` — two-pointer swap.
- `v2_pgas_swap` — distributed array: each rank `aget`s its mirrored region, barrier, writes locally; verified against the deterministic original.
