# 78 — Calculate the Sum of Elements in an Array

## Problem
Sum N elements (04 is the partitioned + allreduce version).

## Input
`argv[1]`: `n`.

## Output
```
sum of <n> elements = <total>
```

## Example
```
mpirun -np 4 ./bin 78_array_sum 1000
sum of 1000 elements = 49500
```

## Solutions
- `v1_basic` — single loop.
