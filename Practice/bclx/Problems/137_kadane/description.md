# 137 — Kadane's Algorithm

## Problem
Maximum sub-array sum in O(n): best_ending_here = max(a[i], best_ending_here + a[i]).

## Input
`argv[1]`: `n`.

## Output
```
Kadane: max subarray sum = <value> over [<s>..<e>]
```

## Example
```
mpirun -np 4 ./bin 137_kadane 12
Kadane: max subarray sum = 132 over [...]
```

## Solutions
- `v1_basic` — O(n) DP with start/end tracking.
