# 106 — Check Whether a Character Is a Vowel or Consonant

## Problem
Read a character and determine whether it is a vowel or consonant (or not an alphabet).

## Input
`argv[1]`: a single character (e.g. `e`).

## Output
```
'c' is a vowel | 'c' is a consonant | 'c' is neither...
```

## Example
```
mpirun -np 4 ./bin 106_vowel_or_consonant e
'e' is a vowel
```

## Solutions
- `v1_basic` — broadcast a char + vowel check.
