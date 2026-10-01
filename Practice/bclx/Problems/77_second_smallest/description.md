# 77 — Find Second Smallest Element in an Array

## Problem
Find the second smallest distinct value (one pass, two trackers).

## Input
`argv[1]`: `n`.

## Output
```
smallest = <s1>, second smallest = <s2>
```

## Example
```
mpirun -np 4 ./bin 77_second_smallest 16
smallest = 0, second smallest = 5
```

## Solutions
- `v1_basic` — one pass with s1/s2 trackers (distinct values).
