# 135 — Find Duplicate in an Array of N+1 Integers

## Problem
Array of n+1 values in [1..n] (i -> a[i%n]); find the duplicate using Floyd's cycle detection on the implicit linked list i -> a[i].

## Input
`argv[1]`: `n`.

## Output
```
duplicate element = <value>
```

## Example
```
mpirun -np 4 ./bin 135_find_duplicate 10
duplicate element = 1
```

## Solutions
- `v1_basic` — Floyd's tortoise and hare.
