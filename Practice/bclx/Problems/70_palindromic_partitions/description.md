# 70 — Find All Possible Palindromic Partitions of the Given String

## Problem
Partition a string so every part is a palindrome (backtracking over cut positions).

## Input
`argv[1]`: string (clamped to 12 chars).

## Output
Each partition on its own line (parts separated by spaces).

## Example
```
mpirun -np 4 ./bin 70_palindromic_partitions aab
a a b
aa b
```

## Solutions
- `v1_basic` — backtracking + palindrome check.
