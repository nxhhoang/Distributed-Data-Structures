# 90 — Finding Maximum Scalar Product of Two Vectors

## Problem
max(A · B) = sort(A asc) · sort(B asc).

## Input
`argv[1]`: `n`.

## Output
```
maximum scalar product = <value>
```

## Example
```
mpirun -np 4 ./bin 90_max_scalar_product 8
maximum scalar product = 24132
```

## Solutions
- `v1_basic` — both sorted ascending + dot.
