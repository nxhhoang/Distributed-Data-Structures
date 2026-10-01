# 152 — Smallest Sub-Array with Sum Greater than a Given Value

## Problem
Find the shortest contiguous sub-array whose sum exceeds x (sliding window).

## Input
`argv[1]`: `n`, `argv[2]`: `x`.
All values positive.

## Output
```
smallest subarray with sum > <x>: length <L> over [<s>..<e>]  |  no subarray...
```

## Example
```
mpirun -np 4 ./bin 152_smallest_subarray_sum 12 60
smallest subarray with sum > 60: length 1
```

## Solutions
- `v1_basic` — sliding window (expand right, shrink left).
