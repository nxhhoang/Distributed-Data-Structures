# 149 — Find Longest Consecutive Subsequence

## Problem
Longest run of consecutive integers (not necessarily contiguous in the array). Only values that are run starts (v-1 absent) can start a run.

## Input
`argv[1]`: `n`.

## Output
```
longest consecutive run: <len> (starting at <v>)
```

## Example
```
mpirun -np 4 ./bin 149_longest_consecutive 30
longest consecutive run: 1 (starting at ...)
```

## Solutions
- `v1_basic` — set + run-start check.
- `v2_hashmap` — striped `BCL::HashMap` probing + `aput` composite reduce.
