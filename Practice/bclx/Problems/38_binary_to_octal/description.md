# 38 — Binary to Octal Conversion

## Problem
Convert binary to octal by left-padding to a multiple of 3, then grouping 3 bits per octal digit.

## Input
`argv[1]`: binary string.

## Output
```
binary <s> in octal = <digits>
```

## Example
```
mpirun -np 4 ./bin 38_binary_to_octal 101101
binary 101101 in octal = 55
```

## Solutions
- `v1_basic` — pad + group 3 bits.
