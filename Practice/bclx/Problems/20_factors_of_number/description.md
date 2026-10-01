# 20 — Factor of a Number

## Problem
Find all divisors of N.

## Input
`argv[1]`: `n`.

## Output
Each rank prints divisors in its chunk: `[rank X] <d> divides <n>`.

## Example
```
mpirun -np 4 ./bin 20_factors_of_number 100
[rank 0] 1 divides 100
[rank 0] 2 divides 100
... (9 divisors total)
```

## Solutions
- `v1_print` — partition [1..N] into contiguous chunks; each rank scans and prints.
