# 117 — Remove Brackets from an Algebraic Expression

## Problem
Strip `()[]{}` from a math expression.

## Input
`argv[1]`: expression string.

## Output
```
brackets removed: "<result>"
```

## Example
```
mpirun -np 4 ./bin 117_remove_brackets "(a+b)*[c-d]/{e*(f+g)}"
brackets removed: "a+b*c-d/e*f+g"
```

## Solutions
- `v1_basic` — filter ()[]{}
