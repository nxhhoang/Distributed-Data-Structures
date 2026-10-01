# 114 — Print the Given String in Reverse Order

## Problem
Print a string reversed.

## Input
`argv[1]`: string.

## Output
- `v1`: `"<s>" reversed = "<rev>"`.
- `v2`: reversed chars + `<- reversed (distributed Treiber stack, verified)`.

## Example
```
mpirun -np 4 ./bin 114_reverse_string "hello world"
"hello world" reversed = "dlrow olleh"
```

## Solutions
- `v1_basic` — backwards print.
- `v2_distributed_stack` — phased pushes (rank order) then phased pops (reverse rank order); each rank pops its own chunk reversed; LIFO semantics.
