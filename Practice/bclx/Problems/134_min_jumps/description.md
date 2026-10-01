# 134 — Minimum Number of Jumps to Reach the End of an Array

## Problem
Greedy reach: extend `farthest` and count jumps when the current window ends.

## Input
`argv[1]`: `n` (values 1..5; always reachable).

## Output
```
minimum jumps to the end = <jumps>
```

## Example
```
mpirun -np 4 ./bin 134_min_jumps 12
minimum jumps to the end = 4
```

## Solutions
- `v1_basic` — greedy reach.
