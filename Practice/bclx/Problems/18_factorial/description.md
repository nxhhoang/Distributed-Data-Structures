# 18 — Factorial of a Number

## Problem
Compute N!.

## Input
`argv[1]`: `n` (clamped to 20 for uint64).

## Output
```
<n>! = <value>
```

## Example
```
mpirun -np 4 ./bin 18_factorial 20
20! = 2432902008176640000
```

## Solutions
- `v1_basic` — iterative on rank 0.
- `v2_chain` — each rank computes its chunk's partial product; partials are chained rank-by-rank via `aget`/`aput` mailbox polling.
