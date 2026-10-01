# 21 — Finding Prime Factors of a Number

## Problem
Express N as a product of prime factors.

## Input
`argv[1]`: `n`.

## Output
```
<n> = p1 * p2 * ...  |  <n> = p1^a * p2^b * ...
```

## Example
```
mpirun -np 4 ./bin 21_prime_factors 360360
360360 = 2^3 * 3^2 * 5 * 7 * 11 * 13
```

## Solutions
- `v1_basic` — trial division on rank 0.
- `v2_parallel` — ranks stripe candidate divisors [2..sqrt(n)] in parallel, report prime divisors via fao+aput; rank 0 computes multiplicities.
