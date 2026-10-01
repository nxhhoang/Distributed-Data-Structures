# 133 — Minimize the Maximum Difference Between Heights

## Problem
Each height can be raised or lowered by k; minimize (max - min).

## Input
`argv[1]`: `n`, `argv[2]`: `k`.

## Output
```
minimum possible (max - min) after +/-<k> = <value>
```

## Example
```
mpirun -np 4 ./bin 133_min_max_height_diff 10 6
minimum possible (max - min) after +-6 = 32
```

## Solutions
- `v1_basic` — sort + boundary candidates.
