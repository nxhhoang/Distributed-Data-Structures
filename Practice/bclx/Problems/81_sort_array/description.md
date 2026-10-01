# 81 — Sort the Elements of an Array

## Problem
Sort N elements ascending.

## Input
`argv[1]`: `n`.

## Output
```
sorted: <values>
```

## Example
```
mpirun -np 4 ./bin 81_sort_array 10
sorted: 0 5 17 22 27 32 39 44 61 66 71 78 83 88
```

## Solutions
- `v1_basic` — `std::sort`.
