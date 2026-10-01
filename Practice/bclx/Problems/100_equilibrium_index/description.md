# 100 — Finding Equilibrium Index of an Array

## Problem
Index i is an equilibrium when sum(a[0..i)) == sum(a[i+1..n).

## Input
`argv[1]`: `n`.

## Output
```
equilibrium indices: <i1> <i2> ...  |  equilibrium indices: (none)
```

## Example
```
mpirun -np 4 ./bin 100_equilibrium_index 12
equilibrium indices: ...
```

## Solutions
- `v1_basic` — prefix/suffix sums.
