# 47 — Calculate the Area of a Circle

## Problem
Compute pi * r^2.

## Input
`argv[1]`: `radius` (double).

## Output
```
area of circle with radius <r> = <area>
```

## Example
```
mpirun -np 4 ./bin 47_circle_area 2.5
area of circle with radius 2.500 = 19.634954
```

## Solutions
- `v1_basic` — broadcast a double + M_PI.
