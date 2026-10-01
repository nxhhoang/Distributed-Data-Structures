# 23 — Perfect Number

## Problem
A number is perfect if it equals the sum of its proper divisors (e.g. 28 = 1+2+4+7+14).

## Input
`argv[1]`: `n`.

## Output
```
<n> is a perfect number  |  <n> is NOT a perfect number
```

## Example
```
mpirun -np 4 ./bin 23_perfect_number 28
28 is a perfect number
```

## Solutions
- `v1_basic` — paired-divisor loop (i*i <= n).
