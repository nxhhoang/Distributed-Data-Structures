# 157 — Spiral Traversal on a Matrix

## Problem
Print matrix elements in spiral order (top row, right column, bottom row, left column, shrink).

## Input
`argv[1]`: `R`, `argv[2]`: `C`.

## Output
```
spiral: <values>
```

## Example
```
mpirun -np 4 ./bin 157_spiral_traversal 4 5
spiral: <20 values>
```

## Solutions
- `v1_basic` — shrinking boundaries.
- `v2_striped_rings` — stripe the concentric RINGS over ranks; `allreduce` count check.
