# 113 — Check if the Given String Is Palindrome or Not

## Problem
Check palindrome via two pointers.

## Input
`argv[1]`: string.

## Output
```
"<s>" is a palindrome  |  "<s>" is NOT a palindrome
```

## Example
```
mpirun -np 4 ./bin 113_string_palindrome racecar
"racecar" is a palindrome
```

## Solutions
- `v1_basic` — two pointers.
- `v2_pgas_mirror` — distributed char array; each rank compares its chunk against the mirrored region via `aget`.
