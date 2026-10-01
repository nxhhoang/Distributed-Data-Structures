# 154 — Minimum Number of Operations to Make an Array a Palindrome

## Problem
Two pointers: matching ends move inward; otherwise merge the smaller side into its neighbor (count++ each merge).

## Input
`argv[1]`: `n`.

## Output
```
minimum merge operations to a palindrome = <ops>
```

## Example
```
mpirun -np 4 ./bin 154_min_ops_palindrome 10
minimum merge operations to a palindrome = 7
```

## Solutions
- `v1_basic` — two-pointer merging.
