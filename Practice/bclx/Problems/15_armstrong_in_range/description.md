# 15 — Armstrong Number in a Given Range

## Problem
Find all Armstrong numbers in [L, R].

## Input
`argv[1]`: `L`, `argv[2]`: `R`.

## Output
```
Armstrong numbers in [<L>..<R>]: <count> found
<list>
```

## Example
```
mpirun -np 4 ./bin 15_armstrong_in_range 1 10000
Armstrong numbers in [1..10000]: 16 found
1 2 3 4 5 6 7 8 9 153 370 371 407 1634 8208 9474
```

## Solutions
- `v2_collect` — atomic-append (fao + aput), prints all results.
