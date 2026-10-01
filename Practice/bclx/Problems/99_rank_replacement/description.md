# 99 — Replace Each Element of the Array by Its Rank

## Problem
Replace each element by its rank (1 = smallest, ties share the same rank).

## Input
`argv[1]`: `n`.

## Output
```
original: <values>
by rank: <ranks>
```

## Example
```
mpirun -np 4 ./bin 99_rank_replacement 12
original: ...
by rank: ...
```

## Solutions
- `v1_basic` — sort + index map with ties sharing rank.
