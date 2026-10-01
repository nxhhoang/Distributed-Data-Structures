# 140 — Best Time to Buy and Sell Stock (one transaction)

## Problem
Best profit = a[i] - min(a[0..i) with `a[i] > mn` guard (uint64 no underflow).

## Input
`argv[1]`: `n`.

## Output
```
best profit = <profit> (buy day <b> at <price>, sell day <s> at <price>)
```

## Example
```
mpirun -np 4 ./bin 140_buy_sell_stock 12
best profit = 88
```

## Solutions
- `v1_basic` — one pass with running min (has `a[i] > mn` guard).
