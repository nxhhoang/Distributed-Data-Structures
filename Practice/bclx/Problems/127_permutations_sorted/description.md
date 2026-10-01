# 127 — Print All Permutations of a Given String in Lexicographically Sorted Order

## Problem
Sort the input, then backtrack with a `used[]` array (sorted input + in-order loop produces lexicographic order; duplicates skipped by checking `s[i]==s[i-1] && !used[i-1]`).

## Input
`argv[1]`: string (clamped to 8 chars, e.g. "aab").

## Output
Each permutation on its own line in sorted order.

## Example
```
mpirun -np 4 ./bin 127_permutations_sorted aab
aab
aba
baa
```

## Solutions
- `v1_basic` — sort + used[] backtracking with duplicate skipping.
