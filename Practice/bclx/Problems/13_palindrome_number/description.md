# 13 — Palindrome Number

## Problem
Determine whether a number reads the same forwards and backwards.

## Input
`argv[1]`: `n`.

## Output
```
<n> is a palindrome  |  <n> is NOT a palindrome
```

## Example
```
mpirun -np 4 ./bin 13_palindrome_number 12321
12321 is a palindrome
```

## Solutions
- `v1_basic` — compare against reversed value.
