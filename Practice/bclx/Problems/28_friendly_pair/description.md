# 28 — Friendly Pair

## Problem
Two numbers are friendly (amicable) if the sum of all divisors of each, divided by the number, is equal (i.e. σ(a)/a == σ(b)/b).

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
(<a>, <b>) is a friendly pair  |  (<a>, <b>) is NOT a friendly pair
```

## Example
```
mpirun -np 4 ./bin 28_friendly_pair 30 140
(30, 140) is a friendly pair
```

## Solutions
- `v1_basic` — divisor-sum + cross-multiplication with `__int128`.
- `v2_two_ranks` — rank 0 computes σ(a), rank 1 computes σ(b); exchanged via `aput`.
