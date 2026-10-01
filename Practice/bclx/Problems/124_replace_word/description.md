# 124 — Replacing a Particular Word with Another Word in a String

## Problem
Replace whole words only ("the" does not match "there").

## Input
`argv[1]`: string, `argv[2]`: old word, `argv[3]`: new word.

## Output
```
replaced <n> word(s): "<result>"
```

## Example
```
mpirun -np 4 ./bin 124_replace_word "the cat sat on the mat" the a
replaced 2 word(s): "a cat sat on a mat"
```

## Solutions
- `v1_basic` — space-tokenized replace.
