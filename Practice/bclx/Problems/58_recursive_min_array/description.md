# 58 — Smallest Element in an Array (Recursion)

## Problem
Find the min using recursion (mirror of 57).

## Input
`argv[1]`: `n`.

## Output
```
smallest of <n> elements = <min>
```

## Example
```
mpirun -np 4 ./bin 58_recursive_min_array 1000
smallest of 1000 elements = 0
```

## Solutions
- `v1_basic` — recursion on rank 0.
