# 150 — Trapping Rain Water Problem

## Problem
Water above bar i = min(maxLeft, maxRight) - height[i]. Two-pointer O(n) solution.

## Input
`argv[1]`: `n` (bar heights 0..9).

## Output
```
trapped water = <units> units
```

## Example
```
mpirun -np 4 ./bin 150_trapping_rain 12
trapped water = 36 units
```

## Solutions
- `v1_basic` — two pointers with running maxima.
