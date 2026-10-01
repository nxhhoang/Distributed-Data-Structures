# 89 — Finding Minimum Scalar Product of Two Vectors

## Problem
min(A · B) = sort(A asc) · sort(B desc) (rearrangement inequality).

## Input
`argv[1]`: `n`.

## Output
```
minimum scalar product = <value>
```

## Example
```
mpirun -np 4 ./bin 89_min_scalar_product 16
minimum scalar product = 19466
```

## Solutions
- `v1_basic` — rank 0 sorts + dot.
- `v2_distributed_dot` — rank 0 ships sorted chunks via bulk `rput_sync`; local dot per rank + `allreduce`.
