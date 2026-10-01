# 36 — Decimal to Octal Conversion

## Problem
Convert a decimal number to octal by repeated division by 8.

## Input
`argv[1]`: `n`.

## Output
```
<n> in octal = <digits>
```

## Example
```
mpirun -np 4 ./bin 36_decimal_to_octal 45
45 in octal = 55
```

## Solutions
- `v1_basic` — broadcast + repeated division by 8.
