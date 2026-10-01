# 105 — Balanced Parenthesis Problem (Stack)

## Problem
Check whether a parenthesis string is balanced using a stack.

## Input
`argv[1]`: parenthesis string (e.g. "(()())").

## Output
```
"<s>" is balanced  |  "<s>" is NOT balanced
```

## Example
```
mpirun -np 4 ./bin 105_balanced_parenthesis_stack (()())
"(()())" is balanced (stack check)
```

## Solutions
- `v1_basic` — `std::stack` check on rank 0.
- `v2_distributed_stack` — distributed Treiber stack (`stack/inc/stack_treiber.h`) with NMR memory manager; push/pop are one-sided RMA (remote CAS).
