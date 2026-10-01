# 50 — Convert Digit/Number to Words

## Problem
Convert a number to its English word representation (up to 999,999,999,999).

## Input
`argv[1]`: `n`.

## Output
```
<n> in words: <words>
```

## Example
```
mpirun -np 4 ./bin 50_number_to_words 12345
12345 in words: twelve thousand, three hundred forty five
```

## Solutions
- `v1_basic` — groups of three digits (billion/million/thousand) + under-1000 helper.
