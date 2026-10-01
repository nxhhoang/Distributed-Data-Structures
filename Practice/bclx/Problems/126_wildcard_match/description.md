# 126 — Check if Two Strings Match Where One Contains Wildcard Characters

## Problem
`?` matches exactly one character; `*` matches any sequence (including empty).

## Input
`argv[1]`: string `S`, `argv[2]`: pattern `P`.

## Output
```
"<S>" MATCHES pattern "<P>"  |  "<S>" does NOT match pattern "<P>"
```

## Example
```
mpirun -np 4 ./bin 126_wildcard_match distributedmemory "dist*memory"
"distributedmemory" MATCHES pattern "dist*memory"
```

## Solutions
- `v1_dp` — DP[i][j] over (S[0..i), P[0..j)).
