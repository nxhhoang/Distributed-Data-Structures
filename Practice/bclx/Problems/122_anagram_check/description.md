# 122 — Check if Two Strings Are Anagram or Not

## Problem
Two strings are anagrams if they contain the same letters with the same frequencies.

## Input
`argv[1]`: `a`, `argv[2]`: `b`.

## Output
```
"<a>" and "<b>" are anagrams  |  ...are NOT anagrams
```

## Example
```
mpirun -np 4 ./bin 122_anagram_check listen silent
"listen" and "silent" are anagrams
```

## Solutions
- `v1_basic` — histogram compare.
- `v3_hashmap` — one shared `BCL::HashMap` with +1/-1 counting; anagram ⟺ all counts zero.
