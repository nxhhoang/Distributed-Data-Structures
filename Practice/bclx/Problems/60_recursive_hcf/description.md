# 60 — HCF of Two Numbers Using Recursion

## Problem
Recursive Euclid: hcf(a, b) = (b == 0) ? a : hcf(b, a % b).

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
HCF(<a>, <b>) = <hcf> (recursive Euclid)
```

## Example
```
mpirun -np 4 ./bin 60_recursive_hcf 36 60
HCF(36, 60) = 12 (recursive Euclid)
```

## Solutions
- `v1_basic` — recursive Euclid (same as 31's GCD).
