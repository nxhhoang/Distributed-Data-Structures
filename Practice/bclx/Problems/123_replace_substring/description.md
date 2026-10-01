# 123 — Replace a Sub-string in a String

## Problem
Replace all occurrences of `sub` with `rep` in the input.

## Input
`argv[1]`: string, `argv[2]`: old substring, `argv[3]`: new substring.

## Output
```
replaced <n> occurrence(s): "<result>"
```

## Example
```
mpirun -np 4 ./bin 123_replace_substring "the cat sat on the mat" the a
replaced 2 occurrence(s): "a cat sat on a mat"
```

## Solutions
- `v1_basic` — find + rebuild.
