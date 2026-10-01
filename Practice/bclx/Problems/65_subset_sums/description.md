# 65 — Print Sums of All Subsets in It

## Problem
Generate all 2^N subset sums via include/exclude recursion.

## Input
`argv[1]`: `N` (array generated deterministically).

## Output
- `v1`: `array: <v0> <v1> ...` + `<2^N> subset sums: ...` (sorted).
- `v2`: per-rank striped sums + `total subset sums = <2^N> (expected <2^N> -> MATCH)`.

## Example
```
mpirun -np 4 ./bin 65_subset_sums 4
array: ...
16 subset sums: ...
```

## Solutions
- `v1_basic` — include/exclude recursion on rank 0.
- `v2_striped_prefixes` — stripe the top-level 2^m prefixes (2^m >= P) via bitmask; recurse below depth m locally; `fao` count check.
