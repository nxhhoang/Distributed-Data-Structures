# 35 — Decimal to Binary Conversion

## Problem
Convert a decimal number to binary by repeated division by 2.

## Input
`argv[1]`: `n`.

## Output
```
<n> in binary = <bits>
```

## Example
```
mpirun -np 4 ./bin 35_decimal_to_binary 45
45 in binary = 101101
```

## Solutions
- `v1_basic` — broadcast + repeated division.
