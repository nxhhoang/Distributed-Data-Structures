# 32 — Binary to Decimal Conversion

## Problem
Convert a binary string to its decimal value using Horner's rule (v = v*2 + bit).

## Input
`argv[1]`: binary string (e.g. "101101").

## Output
```
binary <s> = <value> decimal
```

## Example
```
mpirun -np 4 ./bin 32_binary_to_decimal 101101
binary 101101 = 45 decimal
```

## Solutions
- `v1_basic` — broadcast (fixed-buffer struct) + Horner.
- `v2_parallel_positions` — each rank accumulates weights ≡ me (mod P), `allreduce` sum.
