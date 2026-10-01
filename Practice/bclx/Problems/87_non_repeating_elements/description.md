# 87 — Finding Non-Repeating Elements in an Array

## Problem
Find all elements that appear exactly once.

## Input
`argv[1]`: `n`.

## Output
```
non-repeating elements: <val> <val> ...
```

## Example
```
mpirun -np 4 ./bin 87_non_repeating_elements 60
non-repeating elements: ...
```

## Solutions
- `v1_basic` — freq map, count == 1.
- `v2_hashmap` — HashMap counting + local-segment reporting.
