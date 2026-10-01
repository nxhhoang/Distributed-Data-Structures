# 153 — Three-Way Partitioning of an Array Around a Given Value

## Problem
DNF variant: [<low] [low..high] [>high] in one pass.

## Input
`argv[1]`: `n`, `argv[2]`: `low`, `argv[3]`: `high`.

## Output
```
partitioned around [<low>, <high>]: <values>
```

## Example
```
mpirun -np 4 ./bin 153_three_way_partition 16 30 70
partitioned around [30, 70]: ...
```

## Solutions
- `v1_basic` — DNF three-region.
