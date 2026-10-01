# 131 — Find the Union and Intersection of the Two Sorted Arrays

## Problem
Given sorted distinct arrays A and B, find |A ∪ B| and |A ∩ B|.

## Input
`argv[1]`: `n A`, `argv[2]`: `m B`.

## Output
```
union (<size>): <values>
intersection (<size>): <values>
```

## Example
```
mpirun -np 4 ./bin 131_union_intersection 10 12
intersection (3): 0 12 24
union (19): ...
```

## Solutions
- `v1_basic` — merge walk.
- `v2_striped_bsearch` — striped elements of A + binary search in B + `fao` count.
- `v3_hashmap` — striped inserts + `find` probes + join-on-miss.
