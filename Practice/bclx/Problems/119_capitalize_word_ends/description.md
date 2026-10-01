# 119 — Capitalize the First and Last Character of Each Word

## Problem
For each space-delimited word, uppercase the first and last character.
```
"hello brave" → "HellO BravE"
```

## Input
`argv[1]`: string.

## Output
```
word ends capitalized: "<result>"
```

## Example
```
mpirun -np 4 ./bin 119_capitalize_word_ends "hello brave new world"
word ends capitalized: "HellO BravE NeW WorlD"
```

## Solutions
- `v1_basic` — per-word toupper.
