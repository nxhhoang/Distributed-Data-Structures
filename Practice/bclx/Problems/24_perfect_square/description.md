# 24 — Perfect Square

## Problem
Determine whether N is a perfect square.

## Input
`argv[1]`: `n`.

## Output
```
<n> is a perfect square (<n> = <r>^2)  |  <n> is NOT a perfect square
```

## Example
```
mpirun -np 4 ./bin 24_perfect_square 2500
2500 is a perfect square (2500 = 50^2)
```

## Solutions
- `v1_basic` — sqrtl with rounding fix.
