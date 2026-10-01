# 31 — Greatest Common Divisor (GCD)

## Problem
Compute GCD(a, b) using recursive Euclid: gcd(a, b) = gcd(b, a % b), gcd(a, 0) = a.

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
GCD(<a>, <b>) = <gcd>
```

## Example
```
mpirun -np 4 ./bin 31_gcd 48 180
GCD(48, 180) = 12
```

## Solutions
- `v1_recursive` — recursive Euclid (same as 29's HCF but recursive form).
