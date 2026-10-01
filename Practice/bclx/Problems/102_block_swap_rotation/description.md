# 102 — Block Swap Algorithm for Array Rotation

## Problem
Left-rotate by d using the recursive block-swap method (A/B/C region swaps).

## Input
`argv[1]`: `n`, `argv[2]`: `d`.

## Output
```
left-rotated by <d> (block swap): <values>
```

## Example
```
mpirun -np 4 ./bin 102_block_swap_rotation 10 3
left-rotated by 3 (block swap): ...
```

## Solutions
- `v1_basic` — recursive block swap.
