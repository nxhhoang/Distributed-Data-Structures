# 94 — Finding Arrays Are Disjoint or Not

## Problem
Determine whether two arrays have any common element.

## Input
`argv[1]`: `n A`, `argv[2]`: `m B`.

## Output
```
arrays are disjoint  |  arrays are NOT disjoint (common element: <v>)
```

## Example
```
mpirun -np 4 ./bin 94_disjoint_arrays 40 25
arrays are NOT disjoint (common element: ...)
```

## Solutions
- `v1_basic` — set intersection.
- `v2_hashmap` — striped inserts + `find` probes + fao first-reporter.
