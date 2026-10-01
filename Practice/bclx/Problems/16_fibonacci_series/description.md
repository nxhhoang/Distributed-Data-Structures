# 16 — Fibonacci Series Upto nth Term

## Problem
Generate the first N Fibonacci terms and store them in a distributed array (term i at rank i%P, slot i/P).

## Input
`argv[1]`: `n` (number of terms; clamped to 93 for uint64).

## Output
```
first terms: 0 1 1 2 3 5 8 13 21 34 ... (distributed store verified)
```

## Example
```
mpirun -np 4 ./bin 16_fibonacci_series 50
first terms: 0 1 1 2 3 5 8 13 21 34 ... (distributed store verified)
```

## Solutions
- `v1_distributed_store` — rank 0 computes sequentially, `aput_sync` each term to its home rank; all ranks verify by recomputing locally.
