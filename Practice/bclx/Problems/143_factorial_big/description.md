# 143 — Find the Factorial of a Large Number

## Problem
Compute N! using a digit-array representation (handles N >> 20).

## Input
`argv[1]`: `n` (up to 500).

## Output
```
<n>! has <digits> digits: <first 40 digits>...
```

## Example
```
mpirun -np 4 ./bin 143_factorial_big 100
100! has 158 digits: 9332621544...
```

## Solutions
- `v1_basic` — digit-array multiplication with carry.
