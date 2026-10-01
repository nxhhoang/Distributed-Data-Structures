# 71 — Find All N-bit Binary Numbers Having More 1's than 0's

## Problem
Generate all N-bit strings where every prefix has ones >= zeros.

## Input
`argv[1]`: `n` bits (clamped to 14).

## Output
Each string on its own line + (v2) `total = <c> (DFS cross-check = <c> -> MATCH)`.

## Example
```
mpirun -np 4 ./bin 71_nbit_binary_ones 3
111
110
101
total = 3 (DFS cross-check = 3 -> MATCH)
```

## Solutions
- `v1_basic` — backtracking with ones/zeros counters.
- `v2_bfs_fastqueue` — BFS on `BCL::FastQueue<std::string>`; DFS cross-check.
