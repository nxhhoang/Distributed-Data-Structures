# 110 — Toggle Each Character in a String

## Problem
Swap case: lowercase becomes uppercase and vice versa.

## Input
`argv[1]`: string.

## Output
```
toggled: "<result>"
```

## Example
```
mpirun -np 4 ./bin 110_toggle_case "Hello World 42"
toggled: "hELLO wORLD 42"
```

## Solutions
- `v1_basic` — broadcast + case flip.
