# 125 — Count Common Sub-Sequences in Two Strings

## Problem
Count common subsequences of A and B via DP with inclusion-exclusion:
dp[i][j] = dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1] + (A[i-1]==B[j-1] ? dp[i-1][j-1]+1 : 0).

## Input
`argv[1]`: `A`, `argv[2]`: `B`.

## Output
```
common subsequences of "<A>" and "<B>": <count>
```

## Example
```
mpirun -np 4 ./bin 125_common_subsequence_count abc abc
common subsequences of "abc" and "abc": 7
```

## Solutions
- `v1_dp` — O(n*m) DP on rank 0.
