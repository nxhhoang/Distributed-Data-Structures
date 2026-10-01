# 92 — Find All Symmetric Pairs in an Array

## Problem
A pair (a, b) is symmetric when (b, a) also appears in the array of pairs.

## Input
`argv[1]`: number of pairs (values generated deterministically).

## Output
```
symmetric pairs:
  (<a>, <b>) <-> (<a>, <b>)
```

## Example
```
mpirun -np 4 ./bin 92_symmetric_pairs 12
symmetric pairs:
  ...
```

## Solutions
- `v1_basic` — nested scan (data constructed so some pairs mirror each other).
