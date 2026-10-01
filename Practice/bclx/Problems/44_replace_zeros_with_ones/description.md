# 44 — Replace All 0's with 1 in a Given Integer

## Problem
Replace every digit 0 with 1 (e.g. 1005 → 1115).

## Input
`argv[1]`: `n`.

## Output
```
<n> with 0's replaced by 1's = <result>
```

## Example
```
mpirun -np 4 ./bin 44_replace_zeros_with_ones 1005
1005 with 0's replaced by 1's = 1115
```

## Solutions
- `v1_basic` — broadcast + place-value rebuild.
