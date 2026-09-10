# 05 — Sum of Numbers in a Given Range

## Problem
Compute the sum of all integers in [L, R].

## Input
`argv[1]`: `L`, `argv[2]`: `R`.

## Output
```
sum(<L>..<R>) = <total>
```

## Example
```
mpirun -np 4 ./bin 05_sum_in_range 10 1000000
sum(10..1000000) = 500000499955
```

## Solutions
- `v1_allreduce` — partition [L..R] + `bclx::allreduce`.
- `v2_aput_aget` — "manual reduce": each rank `aput_sync` its partial sum into its slot on rank 0; rank 0 reads all slots and sums locally.
