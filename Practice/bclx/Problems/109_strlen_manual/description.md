# 109 — Length of the String Without Using strlen()

## Problem
Count characters manually (62 is the recursive twin).

## Input
`argv[1]`: string.

## Output
```
length of "<s>" = <len>
```

## Example
```
mpirun -np 4 ./bin 109_strlen_manual "hello world"
length of "hello world" = 11
```

## Solutions
- `v1_basic` — loop until '\0'.
