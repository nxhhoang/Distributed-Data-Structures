# 86 — Finding Repeating Elements in an Array

## Problem
Find all elements that appear more than once (with frequencies).

## Input
`argv[1]`: `n`.

## Output
```
repeating elements: <val>(<freq>x) ...
```

## Example
```
mpirun -np 4 ./bin 86_repeating_elements 60
repeating elements: ...
```

## Solutions
- `v1_basic` — freq map, count > 1.
- `v2_hashmap` — local-count + pre-seed + `modify` pattern; local-segment reporting.
