# 62 — Length of the String Using Recursion

## Problem
len(s) = (s[0] == '\0') ? 0 : 1 + len(s+1).

## Input
`argv[1]`: string.

## Output
```
length of "<s>" = <len> (recursion)
```

## Example
```
mpirun -np 4 ./bin 62_recursive_strlen recursion
length of "recursion" = 9 (recursion)
```

## Solutions
- `v1_basic` — recursive strlen.
