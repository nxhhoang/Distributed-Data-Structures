# 04 — Sum of N Numbers

## Problem
Sum N deterministically-generated numbers (a[i] = (i*2654435761+17)%1000).

## Input
`argv[1]`: `N`.

## Output
```
sum of <N> numbers = <total>
```

## Example
```
mpirun -np 4 ./bin 04_sum_of_n_numbers 1000
sum of 1000 numbers = 499500
```

## Solutions
- `v1_allreduce` — striped array partition + `bclx::allreduce`.
- `v2_pgas_verify` — each rank stores its chunk in its own PGAS heap; rank 0 re-reads all chunks remotely via bulk `rget_sync` to verify (prints MATCH).
