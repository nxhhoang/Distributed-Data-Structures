# 101 — Rotation of Elements of Array: Left and Right

## Problem
Rotate left by d (out[i] = a[(i+d)%n]) and right by d (out[i] = a[(i+n-d)%n]).

## Input
`argv[1]`: `n`, `argv[2]`: `d`.

## Output
```
original: <values>
left by <d>: <values>
right by <d>: <values>
```

## Example
```
mpirun -np 4 ./bin 101_rotation_left_right 10 3
original: ...
left by 3: ...
right by 3: ...
```

## Solutions
- `v1_basic` — index arithmetic.
