# 104 — Finding Circular Rotation of an Array by K Positions

## Problem
B is constructed as A rotated by a hidden K; the program finds every K that turns A into B.

## Input
`argv[1]`: `n`, `argv[2]`: hidden K.

## Output
```
B is a circular rotation of A by K in: <K> (hidden K was <k>)
```

## Example
```
mpirun -np 4 ./bin 104_circular_rotation_check 10 3
B is a circular rotation of A by K in: 3 (hidden K was 3)
```

## Solutions
- `v1_basic` — brute-force shift search.
