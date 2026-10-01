# 11 — Sum of Digits of a Number

## Problem
Sum all decimal digits of a non-negative integer.

## Input
`argv[1]`: `n`.

## Output
```
digit sum of <n> = <sum>
```

## Example
```
mpirun -np 4 ./bin 11_sum_of_digits 123456
digit sum of 123456 = 21
```

## Solutions
- `v1_basic` — broadcast + digit loop.
