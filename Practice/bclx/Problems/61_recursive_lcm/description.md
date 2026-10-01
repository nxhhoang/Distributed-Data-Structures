# 61 — LCM of Two Numbers (Recursive gcd)

## Problem
lcm(a, b) = a / recursive_gcd(a, b) * b.

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
LCM(<a>, <b>) = <lcm> (recursive gcd)
```

## Example
```
mpirun -np 4 ./bin 61_recursive_lcm 12 18
LCM(12, 18) = 36 (recursive gcd)
```

## Solutions
- `v1_basic` — lcm built on recursive gcd.
