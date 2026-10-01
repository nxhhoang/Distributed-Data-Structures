# 156 — Median of 2 Sorted Arrays of Different Sizes

## Problem
Generalized partition search over the smaller array.

## Input
`argv[1]`: `n A`, `argv[2]`: `m B`.

## Output
```
median of arrays (<a> + <b> elements) = <value>
```

## Example
```
mpirun -np 4 ./bin 156_median_two_unequal 5 9
median of arrays (5 + 9 elements) = 12.5
```

## Solutions
- `v1_basic` — generalized partition binary search.
