# 69 — Find the Factorial of a Number Using Recursion

## Problem
fact(n) = (n <= 1) ? 1 : n * fact(n-1).

## Input
`argv[1]`: `n` (clamped to 20).

## Output
```
<n>! = <value> (recursion)
```

## Example
```
mpirun -np 4 ./bin 69_recursive_factorial 10
10! = 3628800 (recursion)
```

## Solutions
- `v1_basic` — plain recursion (cross-ref 18 for the distributed chain).
