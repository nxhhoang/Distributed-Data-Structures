# 75 — Find the Smallest Element in an Array

## Problem
Find the minimum of N elements.

## Input
`argv[1]`: `n`.

## Output
```
smallest of <n> elements = <min>
```

## Example
```
mpirun -np 4 ./bin 75_smallest_element 1000
smallest of 1000 elements = 0
```

## Solutions
- `v1_basic` — single scan.
