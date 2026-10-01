# 14 — Armstrong Number

## Problem
A number is an Armstrong number if it equals the sum of each digit raised to the power of the digit count (e.g. 153 = 1^3+5^3+3^3).

## Input
`argv[1]`: `n`.

## Output
```
<n> is an Armstrong number  |  <n> is NOT an Armstrong number
```

## Example
```
mpirun -np 4 ./bin 14_armstrong_number 153
153 is an Armstrong number
```

## Solutions
- `v1_basic` — broadcast + digit-power sum.
