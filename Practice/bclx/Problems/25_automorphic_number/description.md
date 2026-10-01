# 25 — Automorphic Number

## Problem
A number is automorphic if its square ends with the number itself (e.g. 76^2 = 5776).

## Input
`argv[1]`: `n`.

## Output
```
<n> is automorphic  |  <n> is NOT automorphic
```

## Example
```
mpirun -np 4 ./bin 25_automorphic_number 76
76 is automorphic
```

## Solutions
- `v1_basic` — broadcast + 128-bit square check.
