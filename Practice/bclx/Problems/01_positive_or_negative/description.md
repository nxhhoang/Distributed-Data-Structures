# 01 — Positive or Negative Number

## Problem
Read an integer and determine whether it is positive, negative, or zero.

## Input
`argv[1]`: the integer `n` (e.g. `-42`, `7`, `0`).

## Output
```
<n> is Positive   |  <n> is Negative  |  <n> is Zero
```

## Example
```
mpirun -np 4 ./bin 01_positive_or_negative -42
-42 is Negative
```

## Solutions
- `v1_basic` — rank 0 parses, broadcasts to all ranks, each rank classifies.
