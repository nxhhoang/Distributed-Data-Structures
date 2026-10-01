# 163 — Rotate a Matrix by 90 Degrees

## Problem
Clockwise 90: element (r, c) moves to (c, n-1-r). In place: transpose + reverse rows.

## Input
`argv[1]`: `n` (square).

## Output
```
rotated 90 degrees clockwise:
<rows>
```

## Example
```
mpirun -np 4 ./bin 163_rotate_90 4
12  8  4  0
13  9  5  1
...
```

## Solutions
- `v1_basic` — transpose + reverse rows.
- `v2_pgas_scatter` — bijective index map `(r,c) → (c, n-1-r)`; each rank `aput`s to destination owners.
