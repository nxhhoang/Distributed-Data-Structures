# 146 — Find All Elements That Appear More Than N/K Times

## Problem
Find elements with frequency > n/k.

## Input
`argv[1]`: `n`, `argv[2]`: `k`.

## Output
```
elements appearing more than <threshold> times: <val>(<freq>x) ...  (or empty)
```

## Example
```
mpirun -np 4 ./bin 146_more_than_n_by_k 30 4
elements appearing more than 7 times: (none)
```

## Solutions
- `v1_basic` — sort + run count.
- `v2_hashmap` — HashMap counting + local-segment reporting.
