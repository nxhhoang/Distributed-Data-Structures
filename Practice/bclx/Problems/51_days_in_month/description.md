# 51 — Counting the Number of Days in a Given Month of a Year

## Problem
Return the number of days in a month (February honors the leap-year rule).

## Input
`argv[1]`: `month` (1-12), `argv[2]`: `year`.

## Output
```
month <m> of <y> has <d> days
```

## Example
```
mpirun -np 4 ./bin 51_days_in_month 2 2024
month 2 of 2024 has 29 days
```

## Solutions
- `v1_basic` — broadcast + days table + Feb leap rule.
