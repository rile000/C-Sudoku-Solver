# Sudoku Solver in C++

This project is a Sudoku puzzle solver written in C++ that reads a Sudoku puzzle from a text file, solves it, and writes the solution to another text file.

## Features

- Solves any 9x9 Sudoku puzzle.
- Uses a backtracking algorithm and logical candidate elimination to solve puzzles.
- Provides performance statistics such as:
  - Time to load the puzzle.
  - Time to solve the puzzle.
  - Number of cells solved.
  - Number of candidate values considered.

## How to Use

1. **Input File Format:**
   The input file (`sudoku_puzzle.txt`) should contain 9 rows of numbers, with each number representing a cell in the Sudoku puzzle:
   - Use `0` for empty cells.
   - Example:
     ```
     5 3 0 0 7 0 0 0 0
     6 0 0 1 9 5 0 0 0
     0 9 8 0 0 0 0 6 0
     8 0 0 0 6 0 0 0 3
     4 0 0 8 0 3 0 0 1
     7 0 0 0 2 0 0 0 6
     0 6 0 0 0 0 2 8 0
     0 0 0 4 1 9 0 0 5
     0 0 0 0 8 0 0 7 9
     ```
