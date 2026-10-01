# 98 — Sort an Array According to the Order Defined by Another Array

## Problem
Elements in B come first (in B's order), leftovers ascending.

## Input
`argv[1]`: `n A`, `argv[2]`: `m B`.

## Output
```
A sorted by B's order: <values>
B was: <values>
```

## Example
```
mpirun -np 4 ./bin 98_sort_by_other_array 12 5
A sorted by B's order: ...
B was: ...
```

## Solutions
- `v1_basic` — position map + stable sort.
