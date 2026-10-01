# 29 — Highest Common Factor (HCF)

## Problem
Compute HCF(a, b) = GCD(a, b) using iterative Euclid.

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
HCF(<a>, <b>) = <gcd>
```

## Example
```
mpirun -np 4 ./bin 29_hcf 36 60
HCF(36, 60) = 12
```

## Solutions
- `v1_basic` — broadcast + iterative Euclid.
