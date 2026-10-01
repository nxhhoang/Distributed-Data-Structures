# 64 — Print the F(N)th Term (Recursive Sequence)

## Problem
t(1) = 1, t(2) = 1, t(n) = t(n-1)^2 + t(n-2)^2.

## Input
`argv[1]`: `n` (clamped to 8 for uint64).

## Output
```
F(<n>) = <value> (recursion)
```

## Example
```
mpirun -np 4 ./bin 64_recursive_fn_term 6
F(6) = 866 (recursion)
```

## Solutions
- `v1_basic` — plain recursion (exponential tree, n <= 8).
