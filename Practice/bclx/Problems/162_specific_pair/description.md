# 162 — Find a Specific Pair in a Matrix

## Problem
Maximize mat[c][d] - mat[a][b] over all a < c AND b < d. Bottom-right max table + one lookup per cell.

## Input
`argv[1]`: `R`, `argv[2]`: `C`.

## Output
```
best pair: mat(<a>,<b>)=<v> ... mat(<c>,<d>)=<w> -> difference <d>
```

## Example
```
mpirun -np 4 ./bin 162_specific_pair 4 5
best pair: ... -> difference 93
```

## Solutions
- `v1_basic` — bottom-right max table.
