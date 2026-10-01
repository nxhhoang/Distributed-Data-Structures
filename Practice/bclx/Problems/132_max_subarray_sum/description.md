# 132 — Find the Largest Sum Contiguous Sub-Array

## Problem
Brute-force O(n^2): fix a start, extend the end with a running sum.

## Input
`argv[1]`: `n` (values in [-100, 99]).

## Output
```
largest contiguous sum (O(n^2)) = <value>
```

## Example
```
mpirun -np 4 ./bin 132_max_subarray_sum 12
largest contiguous sum (O(n^2)) = 132
```

## Solutions
- `v1_basic` — O(n^2) (compare with 137 Kadane).
