# 97 — Finding Minimum Sum of Absolute Difference of a Given Array

## Problem
Pair the sorted elements two-by-two to minimize the total absolute difference.

## Input
`argv[1]`: `n` (even).

## Output
```
minimum sum of absolute differences = <value>
```

## Example
```
mpirun -np 4 ./bin 97_min_abs_diff_sum 10
minimum sum of absolute differences = 25
```

## Solutions
- `v1_basic` — sort + adjacent pairing.
