# 42 — Maximum Number of Handshakes

## Problem
Every pair of N people shakes hands exactly once: C(N, 2) = N*(N-1)/2.

## Input
`argv[1]`: `n`.

## Output
```
maximum handshakes among <n> people = <count>
```

## Example
```
mpirun -np 4 ./bin 42_max_handshakes 30
maximum handshakes among 30 people = 435
```

## Solutions
- `v1_basic` — broadcast + formula.
