# 10 — Prime Number Within a Given Range

## Problem
Find all prime numbers in [L, R].

## Input
`argv[1]`: `L`, `argv[2]`: `R`.

## Output
- `v1_print`: each rank prints its chunk's primes as `[rank X] Y`.
- `v2_collect`: `primes in [<L>..<R>]: <count> found` + first few primes in completion order.

## Example
```
mpirun -np 4 ./bin 10_prime_in_range 2 200
primes in [2..200]: 46 found
first few: 2 3 5 7 11 13 17 19 23 29
```

## Solutions
- `v1_print` — contiguous partition + per-rank printing.
- `v2_collect` — atomic-append: each hit takes a slot index via one `fao(+1)` and `aput_sync`s the value; no lock.
