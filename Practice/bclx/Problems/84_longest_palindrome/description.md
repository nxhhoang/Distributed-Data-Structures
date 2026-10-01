# 84 — Finding the Longest Palindrome in an Array

## Problem
Find the element with the most digits that is a palindromic number (ties broken by larger value).

## Input
`argv[1]`: `n` (values in [0,150)).

## Output
```
longest palindrome in array = <v> (<d> digits)
```

## Example
```
mpirun -np 4 ./bin 84_longest_palindrome 60
longest palindrome in array = 141 (3 digits)
```

## Solutions
- `v1_basic` — per-element check + best tracking.
- `v2_aput_best` — per-chunk best (digits, value) pair via `aput` into rank 0's slots.
