# 46 — Count Possible Decodings of a Given Digit Sequence

## Problem
"121" decodes as ABA, AU, LA (3 ways). DP: dp[i] = dp[i-1] + dp[i-2] when the two-digit window is valid (10-26).

## Input
`argv[1]`: digit sequence string.

## Output
```
decodings of "<s>": <count>
```

## Example
```
mpirun -np 4 ./bin 46_count_decodings 121
decodings of "121": 3
```

## Solutions
- `v1_dp` — Fibonacci-shaped DP on rank 0.
