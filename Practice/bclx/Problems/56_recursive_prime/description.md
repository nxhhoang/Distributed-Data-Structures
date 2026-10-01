# 56 — Prime Number (Recursion)

## Problem
Check primality via recursion: is_prime(n, d) where d is the current trial divisor.

## Input
`argv[1]`: `n`.

## Output
```
<n> is prime  |  <n> is NOT prime (recursive check)
```

## Example
```
mpirun -np 4 ./bin 56_recursive_prime 1000000007
1000000007 is prime (recursive check)
```

## Solutions
- `v1_basic` — recursive divisor check starting at d=2.
