# 148 — Next Permutation

## Problem
Find the lexicographically next permutation: find the rightmost ascent, swap with the smallest larger element after it, then reverse the suffix.

## Input
`argv[1...]`: the digits (e.g. `1 3 5 4 2`).

## Output
```
input:    <digits>
next:     <digits>
```

## Example
```
mpirun -np 4 ./bin 148_next_permutation 1 3 5 4 2
next:     1 4 2 3 5
```

## Solutions
- `v1_basic` — pivot + swap + reverse.
