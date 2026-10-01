# 34 — Hexadecimal to Decimal Conversion

## Problem
Convert a hex string (0-9, a-f, A-F) to decimal (Horner, base 16).

## Input
`argv[1]`: hex string (e.g. "1A3F").

## Output
```
hex <s> = <value> decimal
```

## Example
```
mpirun -np 4 ./bin 34_hex_to_decimal 1A3F
hex 1A3F = 6719 decimal
```

## Solutions
- `v1_basic` — broadcast + hex_val + Horner base 16.
