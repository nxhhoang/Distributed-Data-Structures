# 118 — Count the Sum of Numbers in a String

## Problem
Extract multi-digit runs and sum them ("abc12def34gh5" → 12 + 34 + 5 = 51).

## Input
`argv[1]`: string with digit runs.

## Output
```
sum of numbers in "<s>" = <total>
```

## Example
```
mpirun -np 4 ./bin 118_sum_numbers_in_string abc12def34gh5
sum of numbers in "abc12def34gh5" = 51
```

## Solutions
- `v1_basic` — multi-digit run parsing.
