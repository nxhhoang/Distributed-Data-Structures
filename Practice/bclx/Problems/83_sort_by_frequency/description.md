# 83 — Sorting Elements of an Array by Frequency

## Problem
Sort elements by frequency (descending), ties by value (ascending).

## Input
`argv[1]`: `n`.

## Output
```
sorted by frequency: <val>(<freq>x) <val>(<freq>x) ...
```

## Example
```
mpirun -np 4 ./bin 83_sort_by_frequency 30
sorted by frequency: ...
```

## Solutions
- `v1_basic` — freq map + stable sort.
