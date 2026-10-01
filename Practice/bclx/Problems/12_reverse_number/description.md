# 12 — Reverse of a Number

## Problem
Reverse the decimal digits of an integer.

## Input
`argv[1]`: `n`.

## Output
```
reverse of <n> = <rev>
```

## Example
```
mpirun -np 4 ./bin 12_reverse_number 12345
reverse of 12345 = 54321
```

## Solutions
- `v1_basic` — broadcast + accumulation loop.
