# 08 — Leap Year or Not

## Problem
Determine whether a given year is a leap year.

## Input
`argv[1]`: `year`.

## Output
```
<year> is a leap year  |  <year> is NOT a leap year
```

## Example
```
mpirun -np 4 ./bin 08_leap_year 2024
2024 is a leap year
```

## Solutions
- `v1_basic` — broadcast + rule check (divisible by 4 and not 100, or by 400).
