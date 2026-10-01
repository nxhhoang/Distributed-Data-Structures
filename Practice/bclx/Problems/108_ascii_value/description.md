# 108 — Find the ASCII Value of a Character

## Problem
Print the ASCII code of a character.

## Input
`argv[1]`: a character.

## Output
```
ASCII value of 'c' = <value>
```

## Example
```
mpirun -np 4 ./bin 108_ascii_value A
ASCII value of 'A' = 65
```

## Solutions
- `v1_basic` — broadcast a char + cast to int.
