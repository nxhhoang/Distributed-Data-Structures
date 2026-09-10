# 07 — Greatest of the Three Numbers

## Problem
Read three integers and print the largest.

## Input
`argv[1]`: `a`, `argv[2]`: `b`, `argv[3]`: `c`.

## Output
```
greatest(<a>, <b>, <c>) = <max>
```

## Example
```
mpirun -np 4 ./bin 07_greatest_of_three 17 42 23
greatest(17, 42, 23) = 42
```

## Solutions
- `v1_basic` — three broadcasts + running max.
