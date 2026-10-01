# 147 — Maximum Profit by Buying and Selling a Share At Most Twice

## Problem
Two passes: `profit_left[i]` = best within [0..i]; `profit_right[i]` = best within [i..n); answer = best split.

## Input
`argv[1]`: `n`.

## Output
```
maximum profit with at most two transactions = <value>
```

## Example
```
mpirun -np 4 ./bin 147_stock_twice 12
maximum profit with at most two transactions = 166
```

## Solutions
- `v1_basic` — forward/backward profit arrays.
