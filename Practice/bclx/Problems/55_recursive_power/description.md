# 55 — Power of a Number (Recursion)

## Problem
Compute b^e using recursion.

## Input
`argv[1]`: `base`, `argv[2]`: `exponent`.

## Output
```
<b>^<e> = <result> (linear recursion | divide and conquer)
```

## Example
```
mpirun -np 4 ./bin 55_recursive_power 2 10
2^10 = 1024 (linear recursion)
```

## Solutions
- `v1_basic` — linear: power(b, e) = b * power(b, e-1).
- `v2_divide_conquer` — halving: power(b, e) = half^2 * (e odd ? b : 1).
