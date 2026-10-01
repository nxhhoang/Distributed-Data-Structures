# 116 — Remove Spaces from a String

## Problem
Remove all space/tab characters.

## Input
`argv[1]`: string.

## Output
```
spaces removed: "<result>"
```

## Example
```
mpirun -np 4 ./bin 116_remove_spaces "hello  brave   new world"
spaces removed: "hellobravenewworld"
```

## Solutions
- `v1_basic` — filter.
