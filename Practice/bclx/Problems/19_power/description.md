# 19 — Power of a Number

## Problem
Compute base^exponent.

## Input
`argv[1]`: `base`, `argv[2]`: `exponent`.

## Output
```
result = <value>
```

## Example
```
mpirun -np 4 ./bin 19_power 2 40
result = 1099511627776
```

## Solutions
- `v1_fast_pow` — square-and-multiply (O(log e)) on rank 0.
