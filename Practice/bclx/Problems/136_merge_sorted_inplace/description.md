# 136 — Merge 2 Sorted Arrays Without Using Extra Space

## Problem
Sort the concatenation of A (n items) and B (m items) in place using the gap (shell-sort-like) method.

## Input
`argv[1]`: `n A`, `argv[2]`: `m B`.

## Output
```
merged A: <first n values>
merged B: <last m values>
```

## Example
```
mpirun -np 4 ./bin 136_merge_sorted_inplace 6 7
merged A: 1 2 3 4 5 6
merged B: 7 8 9 10 11 12 14
```

## Solutions
- `v1_basic` — gap method (gap halves each pass).
