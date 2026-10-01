# 72 — Given a Set of Positive Integers, Find All Its Subsets

## Problem
Generate all 2^N subsets via include/exclude recursion (prints each set).

## Input
`argv[1]`: `N` (values generated deterministically).

## Output
Each subset as `{ v1 v2 ... }` on its own line.

## Example
```
mpirun -np 4 ./bin 72_all_subsets 3
{ }
{ v0 }
{ v0 v1 }
...
(8 subsets total)
```

## Solutions
- `v1_basic` — include/exclude recursion (65/v2 shows the striped variant).
