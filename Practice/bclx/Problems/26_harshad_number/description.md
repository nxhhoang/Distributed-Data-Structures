# 26 — Harshad Number

## Problem
A Harshad (Niven) number is divisible by the sum of its digits (e.g. 18 / (1+8) = 2).

## Input
`argv[1]`: `n`.

## Output
```
<n> is a Harshad number  |  <n> is NOT a Harshad number
```

## Example
```
mpirun -np 4 ./bin 26_harshad_number 18
18 is a Harshad number
```

## Solutions
- `v1_basic` — broadcast + digit sum divisibility.
