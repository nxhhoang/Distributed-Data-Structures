# 66 — Last Non-Zero Digit in Factorial

## Problem
Find the last non-zero digit of N!.

## Input
`argv[1]`: `n`.

## Output
```
last non-zero digit of <n>! = <d> (recursion)
```

## Example
```
mpirun -np 4 ./bin 66_last_nonzero_digit_factorial 25
last non-zero digit of 25! = 4 (recursion)
```

## Solutions
- `v1_basic` — classic D(n) recursion with table {1,1,2,6,4,2,2,4,2,8}.
