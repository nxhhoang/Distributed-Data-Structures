# 121 — Find Non-Repeating Characters in a String

## Problem
Find all letters appearing exactly once (case-insensitive).

## Input
`argv[1]`: string.

## Output
```
non-repeating letters in "<s>": <letters>
```

## Example
```
mpirun -np 4 ./bin 121_non_repeating_chars "swiss cheese"
non-repeating letters in "swiss cheese": c h i w
```

## Solutions
- `v1_basic` — freq table, count == 1.
