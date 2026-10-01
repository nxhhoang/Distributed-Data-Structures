# 73 — Remove All Adjacent Duplicate Characters Recursively

## Problem
Repeatedly remove runs of >= 2 equal adjacent characters until stable (e.g. "abssbe" → "ae").

## Input
`argv[1]`: string.

## Output
```
"<s>" -> "<result>" (recursive run-collapse)
```

## Example
```
mpirun -np 4 ./bin 73_remove_adjacent_duplicates abssbe
"abssbe" -> "ae" (recursive run-collapse)
```

## Solutions
- `v1_basic` — run-collapse recursion.
