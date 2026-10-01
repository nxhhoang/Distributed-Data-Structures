# 145 — Rearrange the Array in Alternating Positive and Negative Items with O(1) Extra Space

## Problem
Right-rotation method: at each position with the wrong sign, find the next slot with the wrong sign and right-rotate the segment between them. O(n^2), O(1) space.

## Input
`argv[1]`: `n`.

## Output
```
alternating: <values>
```

## Example
```
mpirun -np 4 ./bin 145_alternating_pos_neg 12
alternating: ...
```

## Solutions
- `v1_basic` — right-rotation insertion.
