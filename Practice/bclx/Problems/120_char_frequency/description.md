# 120 — Calculate Frequency of Characters in a String

## Problem
Count letters (case-insensitive, 26 buckets).

## Input
`argv[1]`: string.

## Output
```
frequencies in "<s>":
  <c> -> <freq>
```

## Example
```
mpirun -np 4 ./bin 120_char_frequency mississippi
frequencies in "mississippi":
  i -> 4
  s -> 4
  ...
```

## Solutions
- `v1_basic` — 26-bucket table.
- `v2_pgas_histogram` — local histogram + batched `fao` into rank 0's table.
