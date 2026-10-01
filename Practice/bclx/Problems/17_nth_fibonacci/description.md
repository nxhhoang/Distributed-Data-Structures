# 17 — Find the Nth Term of the Fibonacci Series

## Problem
Compute F(n) where F(0)=0, F(1)=1.

## Input
`argv[1]`: `n` (clamped to 93).

## Output
```
F(<n>) = <value>
```

## Example
```
mpirun -np 4 ./bin 17_nth_fibonacci 50
F(50) = 12586269025
```

## Solutions
- `v1_iterative` — O(n) loop on rank 0.
- `v2_fast_doubling` — O(log n) fast doubling on rank 0; cross-checked against iterative on another rank (prints MATCH).
