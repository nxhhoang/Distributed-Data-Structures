# 85 — Counting Distinct Elements in an Array

## Problem
Count the number of distinct values.

## Input
`argv[1]`: `n`.

## Output
```
distinct elements among <n> = <count>
```

## Example
```
mpirun -np 4 ./bin 85_count_distinct 30
distinct elements among 30 = 30
```

## Solutions
- `v1_basic` — set on rank 0.
- `v2_hashmap` — striped `BCL::HashMap` inserts, count occupied slots per local segment.
