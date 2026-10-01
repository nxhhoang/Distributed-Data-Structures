# 158 — Search an Element in a Matrix

## Problem
Staircase walk from top-right corner: eliminate a row or column each comparison — O(R+C).

## Input
`argv[1]`: `R`, `argv[2]`: `C`, `argv[3]`: `target`.

## Output
```
<target> found at row <r>, col <c>  |  <target> not in the matrix
```

## Example
```
mpirun -np 4 ./bin 158_search_matrix 4 5 21
21 found at row 2, col 0
```

## Solutions
- `v1_basic` — staircase walk.
- `v2_striped_rows` — striped rows + binary search per row + fao first-reporter.
