# 115 — Remove All Characters from String Except Alphabets

## Problem
Keep only alphabetic characters.

## Input
`argv[1]`: string.

## Output
```
alphabets only: "<result>"
```

## Example
```
mpirun -np 4 ./bin 115_keep_alphabets_only "Hi, 2024 World!"
alphabets only: "HiWorld"
```

## Solutions
- `v1_basic` — isalpha filter.
