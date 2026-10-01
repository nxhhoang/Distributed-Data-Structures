# 111 — Count the Number of Vowels

## Problem
Count vowel occurrences in a string.

## Input
`argv[1]`: string.

## Output
```
"<s>" contains <N> vowel(s)
```

## Example
```
mpirun -np 4 ./bin 111_count_vowels "distributed memory"
"distributed memory" contains 6 vowels
```

## Solutions
- `v1_basic` — single loop.
- `v2_striped` — striped positions (i % P == me) + `allreduce`.
