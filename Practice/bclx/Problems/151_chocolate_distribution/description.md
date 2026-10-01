# 151 — Chocolate Distribution Problem

## Problem
Give m students one packet each from n packets; minimize the difference between the most and least chocolate — the best m packets are consecutive in sorted order.

## Input
`argv[1]`: `n`, `argv[2]`: `m`.

## Output
```
packets for <m> students: <values> (min difference = <d>)
```

## Example
```
mpirun -np 4 ./bin 151_chocolate_distribution 12 4
packets for 4 students: ... (min difference = 11)
```

## Solutions
- `v1_basic` — sort + sliding window of m.
