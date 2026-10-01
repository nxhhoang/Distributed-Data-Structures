# 33 — Octal to Decimal Conversion

## Problem
Convert an octal string to decimal (Horner, base 8).

## Input
`argv[1]`: octal string (e.g. "157").

## Output
```
octal <s> = <value> decimal
```

## Example
```
mpirun -np 4 ./bin 33_octal_to_decimal 157
octal 157 = 111 decimal
```

## Solutions
- `v1_basic` — broadcast + Horner base 8.
