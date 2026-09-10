# 02 — Even or Odd Number

## Problem
Read an integer and determine whether it is even or odd.

## Input
`argv[1]`: the integer `n`.

## Output
```
<n> is Even  |  <n> is Odd
```

## Example
```
mpirun -np 4 ./bin 02_even_or_odd 7
7 is Odd
```

## Solutions
- `v1_basic` — broadcast + modulo check.
- `v2_batch_count` — partition [lo..hi] over ranks, count even/odd with `fao` counters on rank 0.
