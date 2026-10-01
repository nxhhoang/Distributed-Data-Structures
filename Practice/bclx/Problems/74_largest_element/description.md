# 74 — Find the Largest Element in an Array

## Problem
Find the maximum of N elements (57/v2 has the distributed recursive variant).

## Input
`argv[1]`: `n`.

## Output
```
largest of <n> elements = <max>
```

## Example
```
mpirun -np 4 ./bin 74_largest_element 1000
largest of 1000 elements = 999
```

## Solutions
- `v1_basic` — single scan on rank 0.
