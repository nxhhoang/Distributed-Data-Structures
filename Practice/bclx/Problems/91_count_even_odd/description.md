# 91 — Counting the Number of Even and Odd Elements in an Array

## Problem
Count even and odd elements.

## Input
`argv[1]`: `n`.

## Output
```
even = <E>, odd = <O> (of <n>)
```

## Example
```
mpirun -np 4 ./bin 91_count_even_odd 30
even = 15, odd = 15 (of 30)
```

## Solutions
- `v1_basic` — single loop.
- `v2_fao_counters` — per-rank counts + batched `fao` into rank 0's two counters.
