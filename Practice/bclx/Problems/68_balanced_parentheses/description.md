# 68 — Generate All Combinations of Well-Formed Parentheses

## Problem
Generate all balanced strings with N pairs of parentheses via backtracking.

## Input
`argv[1]`: `n` (pairs, clamped to 8).

## Output
Each string on its own line + (v2) `total = <Catalan(n)> (Catalan(<n>) = <C> -> MATCH)`.

## Example
```
mpirun -np 4 ./bin 68_balanced_parentheses 3
((()))
(()())
(())()
()(())  ()
()()()
total = 5 (Catalan(3) = 5 -> MATCH)
```

## Solutions
- `v1_basic` — backtracking with open/close counters.
- `v2_bfs_fastqueue` — level-synchronized BFS on `BCL::FastQueue<std::string>` (strings ride ObjectContainer serialization); barrier -> size() -> pop slice -> push children; Catalan cross-check.
