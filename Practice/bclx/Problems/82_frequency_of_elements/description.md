# 82 — Finding the Frequency of Elements in an Array

## Problem
Count occurrences of each distinct value.

## Input
`argv[1]`: `n`.

## Output
```
frequencies over <n> elements:
  <value> -> <count>
...
```

## Example
```
mpirun -np 4 ./bin 82_frequency_of_elements 60
frequencies over 60 elements:
  0 -> 1
  ...
```

## Solutions
- `v1_basic` — map counting on rank 0.
- `v2_pgas_histogram` — LOCAL histogram per chunk + batched `fao` into rank 0's table.
- `v3_hashmap` — `BCL::HashMap` with race-free pre-seed + atomic `modify`.
