# 63 — Print All Permutations of a String

## Problem
Generate all N! permutations via swap-recursion (backtracking).

## Input
`argv[1]`: string (clamped to 8 chars).

## Output
- `v1`: each permutation on its own line.
- `v2`: `[rank X] <perm>` + verification `total permutations = <N!> (expected <N!> -> MATCH)`.

## Example
```
mpirun -np 4 ./bin 63_string_permutations ABCD
[rank 0] ABCD
...
total permutations = 24 (expected 24 -> MATCH)
```

## Solutions
- `v1_basic` — swap recursion on rank 0.
- `v2_striped_branches` — stripe the top-level branches of the recursion tree; each rank explores its branches locally; `allreduce` count check.
