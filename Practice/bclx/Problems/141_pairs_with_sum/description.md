# 141 — Find All Pairs on Integer Array Whose Sum Is Equal to a Given Number

## Problem
Count pairs (i < j) with a[i] + a[j] == target.

## Input
`argv[1]`: `n`, `argv[2]`: `target`.

## Output
```
pairs summing to <target>: <count>
```

## Example
```
mpirun -np 4 ./bin 141_pairs_with_sum 16 70
pairs summing to 70: 0
```

## Solutions
- `v1_basic` — hash count.
- `v2_striped` — striped outer loop + `fao` count.
- `v3_hashmap` — one-pass seen-map via `BCL::HashMap`.
