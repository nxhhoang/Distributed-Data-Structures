# 142 — Find if There Is Any Sub-Array With Sum Equal to Zero

## Problem
A sub-array sums to 0 iff two prefix sums are equal. Data includes a guaranteed (5, -5) pair.

## Input
`argv[1]`: `n`.

## Output
```
zero-sum subarray exists, ending at index <i>  |  no zero-sum subarray
```

## Example
```
mpirun -np 4 ./bin 142_zero_sum_subarray 10
zero-sum subarray exists, ending at index <i>
```

## Solutions
- `v1_basic` — prefix sums + set on rank 0.
- `v2_hashmap` — prefix-sum markers in `BCL::HashMap`.
