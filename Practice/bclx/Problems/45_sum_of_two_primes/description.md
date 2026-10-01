# 45 — Can a Number Be Expressed as a Sum of Two Prime Numbers

## Problem
Find p, q both prime such that p + q = n (Goldbach-style check).

## Input
`argv[1]`: `n`.

## Output
```
<n> = <p> + <q> (both prime)  |  <n> cannot be expressed as a sum of two primes
```

## Example
```
mpirun -np 4 ./bin 45_sum_of_two_primes 74
74 = 3 + 71 (both prime)
```

## Solutions
- `v1_basic` — rank 0 scans p in [2..n/2].
- `v2_parallel` — striped p candidates; first reporter wins via `fao` + `aput`.
