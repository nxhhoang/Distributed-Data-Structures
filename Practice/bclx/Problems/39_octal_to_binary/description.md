# 39 — Octal to Binary Conversion

## Problem
Convert octal to binary by expanding each octal digit to 3 bits; strip leading zeros.

## Input
`argv[1]`: octal string.

## Output
```
octal <s> in binary = <bits>
```

## Example
```
mpirun -np 4 ./bin 39_octal_to_binary 345
octal 345 in binary = 11100101
```

## Solutions
- `v1_basic` — per-digit 3-bit expansion + leading-zero strip.
