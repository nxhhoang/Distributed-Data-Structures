# 103 — Juggling Algorithm for Array Rotation

## Problem
Left-rotate by d using gcd cycles: each cycle moves items `d` positions ahead.

## Input
`argv[1]`: `n`, `argv[2]`: `d`.

## Output
```
left-rotated by <d> (juggling, <g> cycles): <values>
```

## Example
```
mpirun -np 4 ./bin 103_juggling_rotation 10 3
left-rotated by 3 (juggling, 1 cycles): ...
```

## Solutions
- `v1_basic` — gcd cycles.
