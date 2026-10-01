# 49 — Calculate the Number of Digits in an Integer

## Problem
Count the decimal digits of n.

## Input
`argv[1]`: `n`.

## Output
```
<n> has <d> digit(s)
```

## Example
```
mpirun -np 4 ./bin 49_count_digits 123456
123456 has 6 digits
```

## Solutions
- `v1_basic` — broadcast + digit loop.
