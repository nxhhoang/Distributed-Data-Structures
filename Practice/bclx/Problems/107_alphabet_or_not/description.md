# 107 — Check Whether a Character Is an Alphabet or Not

## Problem
Determine if a character is a letter.

## Input
`argv[1]`: a character.

## Output
```
'c' is an alphabet  |  'c' is NOT an alphabet
```

## Example
```
mpirun -np 4 ./bin 107_alphabet_or_not q
'q' is an alphabet
```

## Solutions
- `v1_basic` — broadcast + isalpha.
