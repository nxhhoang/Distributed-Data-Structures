# 93 — Find Maximum Product Sub-Array

## Problem
Find the contiguous sub-array with the maximum product (handles negatives via max/min swap).

## Input
`argv[1]`: `n` (values in [-9, 9]).

## Output
```
maximum product sub-array = <value>
```

## Example
```
mpirun -np 4 ./bin 93_max_product_subarray 12
maximum product sub-array = 3360
```

## Solutions
- `v1_basic` — Kadane with max/min tracking.
