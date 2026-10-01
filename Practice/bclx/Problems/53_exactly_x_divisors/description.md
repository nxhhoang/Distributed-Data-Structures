# 53 — Finding the Number of Integers Which Have Exactly x Divisors

## Problem
Count integers in [1..N] with exactly x divisors.

## Input
`argv[1]`: `N`, `argv[2]`: `x`.

## Output
```
numbers in [1..<N>] with exactly <x> divisors: <count>
```

## Example
```
mpirun -np 4 ./bin 53_exactly_x_divisors 20 4
numbers in [1..20] with exactly 4 divisors: 5
```

## Solutions
- `v1_basic` — rank 0 + paired-divisor count.
- `v2_allreduce` — partition [1..N], count per rank, `allreduce`.
