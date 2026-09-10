# 06 — Greatest of Two Numbers

## Problem
Read two integers and print the larger one.

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
greatest(<a>, <b>) = <max>
```

## Example
```
mpirun -np 4 ./bin 06_greatest_of_two 17 42
greatest(17, 42) = 42
```

## Solutions
- `v1_basic` — two broadcasts + comparison.
- `v2_pgas` — both values live in rank 0's PGAS heap; every rank `aget_sync`s both and computes the max itself.
