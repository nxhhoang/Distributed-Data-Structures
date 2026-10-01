# 57 — Largest Element in an Array (Recursion)

## Problem
Find the max using recursion: max(a, i) = max(a[i], max(a, i-1)).

## Input
`argv[1]`: `n` (array generated deterministically).

## Output
```
largest of <n> elements = <max>
```

## Example
```
mpirun -np 4 ./bin 57_recursive_max_array 1000
largest of 1000 elements = 999
```

## Solutions
- `v1_basic` — recursion on rank 0.
- `v2_allreduce` — PGAS chunks + recursive max per chunk + `aput` partial maxima to rank 0.
