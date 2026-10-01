# 41 — Permutations: n People Occupy r Seats

## Problem
Compute P(n, r) = n!/(n-r)! as a falling product.

## Input
`argv[1]`: `n`, `argv[2]`: `r`.

## Output
```
P(<n>, <r>) = <value>
```

## Example
```
mpirun -np 4 ./bin 41_permutations 10 3
P(10, 3) = 720
```

## Solutions
- `v1_basic` — falling product on rank 0.
- `v2_chain` — stripe factors across ranks; chain partial products via `aget`/`aput` mailbox polling.
