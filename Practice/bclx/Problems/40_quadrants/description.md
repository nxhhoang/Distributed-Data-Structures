# 40 — Quadrants in Which a Given Coordinate Lies

## Problem
Given (x, y), determine which quadrant, axis, or origin the point lies in.

## Input
`argv[1]`: `x`, `argv[2]`: `y`.

## Output
```
(<x>, <y>) lies in Quadrant 1|2|3|4 | the origin | the X-axis | the Y-axis
```

## Example
```
mpirun -np 4 ./bin 40_quadrants -3 5
(-3, 5) lies in Quadrant 2
```

## Solutions
- `v1_basic` — broadcast both coordinates + sign/axis check.
