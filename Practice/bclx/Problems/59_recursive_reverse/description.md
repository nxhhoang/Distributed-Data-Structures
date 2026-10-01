# 59 — Reversing a Number (Recursion)

## Problem
Reverse using accumulator recursion: rev(n, acc) = rev(n/10, acc*10 + n%10).

## Input
`argv[1]`: `n`.

## Output
```
reverse of <n> = <rev> (recursion)
```

## Example
```
mpirun -np 4 ./bin 59_recursive_reverse 12345
reverse of 12345 = 54321 (recursion)
```

## Solutions
- `v1_basic` — accumulator recursion.
