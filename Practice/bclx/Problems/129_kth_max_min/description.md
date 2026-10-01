# 129 — Find the Kth Max and Min Element of an Array

## Problem
Find the k-th smallest and k-th largest element.

## Input
`argv[1]`: `n`, `argv[2]`: `k`.

## Output
```
<k>-th smallest = <A>, <k>-th largest = <B>
```

## Example
```
mpirun -np 4 ./bin 129_kth_max_min 16 3
3-th smallest = 105, 3-th largest = 822
```

## Solutions
- `v1_basic` — sort + index.
- `v2_own_heap` — hand-rolled size-k heaps per rank + `aput` candidate merge.
