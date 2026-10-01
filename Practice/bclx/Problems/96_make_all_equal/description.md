# 96 — Determine Can All Numbers of an Array Be Made Equal

## Problem
Given operations "multiply by 2" and "multiply by 3", determine if all numbers can reach the same value (strip factors 2 and 3, compare remainders).

## Input
`argv[1]`: `n`.

## Output
```
all <n> numbers can be made equal (x2/x3 multiplications)  |  ...canNOT...
```

## Example
```
mpirun -np 4 ./bin 96_make_all_equal 8
all 8 numbers canNOT be made equal (x2/x3 multiplications)
```

## Solutions
- `v1_basic` — strip 2s and 3s, compare all remainders.
