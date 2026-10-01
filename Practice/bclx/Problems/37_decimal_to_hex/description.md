# 37 — Decimal to Hexadecimal Conversion

## Problem
Convert a decimal number to hex (digits 0123456789ABCDEF).

## Input
`argv[1]`: `n`.

## Output
```
<n> in hexadecimal = <digits>
```

## Example
```
mpirun -np 4 ./bin 37_decimal_to_hex 6715
6715 in hexadecimal = 1A3B
```

## Solutions
- `v1_basic` — broadcast + repeated division by 16 + digit table.
