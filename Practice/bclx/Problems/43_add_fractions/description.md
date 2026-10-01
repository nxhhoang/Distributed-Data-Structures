# 43 — Addition of Two Fractions

## Problem
Compute a/b + c/d, reduce with GCD.

## Input
`argv[1..4]`: `a`, `b`, `c`, `d` (denominators non-zero).

## Output
```
<a>/<b> + <c>/<d> = <num>/<den>
```

## Example
```
mpirun -np 4 ./bin 43_add_fractions 1 2 3 4
1/2 + 3/4 = 5/4
```

## Solutions
- `v1_basic` — 4 broadcasts + lcm denominator + gcd reduction.
