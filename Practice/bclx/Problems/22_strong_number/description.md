# 22 — Strong Number

## Problem
A number is strong if it equals the sum of factorials of its digits (e.g. 145 = 1! + 4! + 5!).

## Input
`argv[1]`: `n`.

## Output
```
<n> is a strong number  |  <n> is NOT a strong number
```

## Example
```
mpirun -np 4 ./bin 22_strong_number 145
145 is a strong number
```

## Solutions
- `v1_basic` — broadcast + digit factorial table.
