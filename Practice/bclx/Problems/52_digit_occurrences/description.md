# 52 — Finding the Number of Times Digit x Occurs in a Given Input

## Problem
Count occurrences of a digit in a number (or across a range of numbers).

## Input
- `v1`: `argv[1]`: `n`, `argv[2]`: digit `x` (0-9).
- `v2`: `argv[1..3]`: `L`, `R`, digit `x`.

## Output
```
digit <x> occurs <c> time(s) in <n>        (v1)
digit <x> occurs <c> time(s) across [<L>..<R>]  (v2)
```

## Example
```
mpirun -np 4 ./bin 52_digit_occurrences 1 1000 7
digit 7 occurs 300 times across [1..1000]
```

## Solutions
- `v1_basic` — broadcast + digit loop.
- `v2_batch_range` — partition [L..R], count per rank, `fao` on rank 0's counter.
