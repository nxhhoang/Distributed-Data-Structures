# 48 — Find the Prime Numbers Between 1 and 100

## Problem
Find all primes in [1, 100] — there are exactly 25.

## Input
(None required).

## Output
- Each rank prints its striped primes: `[rank X] Y`.
- Total: `total primes in [1..100]: 25`.

## Example
```
mpirun -np 4 ./bin 48_primes_1_to_100
[rank 0] 2
...
total primes in [1..100]: 25 (striped over 4 ranks)
```

## Solutions
- `v1_striped` — striped x ≡ me (mod P); contrast with contiguous chunks in problem 10.
