# 138 — Merge Intervals

## Problem
Merge overlapping intervals in a sorted set: overlap when next.start <= current.end.

## Input
`argv[1]`: `n` (intervals generated deterministically).

## Output
```
<n> intervals in -> <m> merged: [<s>, <e>] ...
```

## Example
```
mpirun -np 4 ./bin 138_merge_intervals 8
8 intervals in -> 3 merged: [0, 3] [5, 24] [25, 44]
```

## Solutions
- `v1_basic` — sort by start + merge.
