# 54 — Finding the Roots of a Quadratic Equation

## Problem
Solve ax^2 + bx + c = 0 (handles linear, real, double, and complex roots).

## Input
`argv[1..3]`: `a`, `b`, `c` (doubles).

## Output
```
two real roots: <r1> and <r2>  |  double root: <r>  |  complex roots: <re> ± <im>i  |  linear, root = <r>
```

## Example
```
mpirun -np 4 ./bin 54_quadratic_roots 1 -5 6
two real roots: 3.000000 and 2.000000
```

## Solutions
- `v1_basic` — broadcast 3 doubles + discriminant.
