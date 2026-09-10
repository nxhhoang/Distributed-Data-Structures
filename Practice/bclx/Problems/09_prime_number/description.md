# 09 — Prime Number

## Problem
Determine whether a given number is prime.

## Input
`argv[1]`: `n`.

## Output
```
<n> is prime  |  <n> is NOT prime
```

## Example
```
mpirun -np 4 ./bin 09_prime_number 1000000007
1000000007 is prime
```

## Solutions
- `v1_basic` — trial division on rank 0.
- `v2_parallel` — stripe divisors [2..sqrt(n)] across ranks (`d = 2+me, step P`); each rank counts hits; `allreduce` sum == 0 means prime.
