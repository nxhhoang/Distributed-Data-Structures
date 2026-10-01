# 27 — Abundant Number

## Problem
A number is abundant if the sum of its proper divisors exceeds it (e.g. 12: 1+2+3+4+6 = 16 > 12).

## Input
`argv[1]`: `n`.

## Output
```
<n> is an abundant number  |  <n> is NOT an abundant number
```

## Example
```
mpirun -np 4 ./bin 27_abundant_number 12
12 is an abundant number
```

## Solutions
- `v1_basic` — paired-divisor loop.
