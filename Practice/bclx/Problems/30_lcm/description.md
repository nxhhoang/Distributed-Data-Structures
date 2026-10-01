# 30 — Lowest Common Multiple (LCM)

## Problem
Compute LCM(a, b) = a / gcd(a,b) * b.

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
LCM(<a>, <b>) = <lcm>
```

## Example
```
mpirun -np 4 ./bin 30_lcm 12 18
LCM(12, 18) = 36
```

## Solutions
- `v1_basic` — broadcast + HCF-based formula.
