# 67 — Find the Nth Row of Pascal's Triangle

## Problem
Print row N of Pascal's triangle.

## Input
`argv[1]`: `n`.

## Output
```
row <n> of Pascal's triangle: <C(n,0)> <C(n,1)> ... <C(n,n)>
```

## Example
```
mpirun -np 4 ./bin 67_pascal_row 6
row 6 of Pascal's triangle: 1 6 15 20 15 6 1
```

## Solutions
- `v1_basic` — recursive binomial C(n,k) = C(n-1,k-1) + C(n-1,k).
- `v2_striped` — striped k + multiplicative formula per element, `aput` into rank 0's row array.
