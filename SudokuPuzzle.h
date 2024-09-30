#pragma once

#include <chrono>
#include <vector>

struct Cell {
    std::vector<int> candidateList;
    int value = 0;
    bool predefined;
};


class SudokuPuzzle {
public:
    SudokuPuzzle();
    SudokuPuzzle(const SudokuPuzzle& other);  // Copy constructor
    SudokuPuzzle& operator=(const SudokuPuzzle& other);  // Copy assignment operator
    ~SudokuPuzzle();

    void load(const char filenameIn[]);
    void solve();
    void output(const char filenameOut[]) const;

private:
    void buildCandidateLists();
    void identifySingles();

    std::chrono::duration<double, std::micro> _loadTime;
    std::chrono::duration<double, std::micro> _solveTime;
    Cell puzzleGrid[9][9];
    int candidateValuesConsidered;
    int cellsSolved;
    int passes;
};
