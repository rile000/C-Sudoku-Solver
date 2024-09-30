#include "SudokuPuzzle.h"
#include <iostream>
#include <fstream>



SudokuPuzzle::SudokuPuzzle() 
   : _loadTime(std::chrono::duration<double>(0)),
     _solveTime(std::chrono::duration<double>(0)),
     candidateValuesConsidered(0), 
     cellsSolved(0),
     passes(0) {
}




SudokuPuzzle::~SudokuPuzzle() {
    try {
        std::cout << "Time to load: " << _loadTime.count() << " microseconds" << std::endl;
        std::cout << "Time to solve: " << _solveTime.count() << " microseconds" << std::endl;
        std::cout << "Cells solved: " << cellsSolved << std::endl;
        std::cout << "Passes through the grid: " << passes << std::endl;
        std::cout << "Total candidate values considered: " << candidateValuesConsidered << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error in SudokuPuzzle destructor: " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error in SudokuPuzzle destructor." << std::endl;
    }
}


void SudokuPuzzle::load(const char filenameIn[]) {
    

    std::ifstream inputFile(filenameIn);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filenameIn << std::endl;
        return;
    }
    const auto start = std::chrono::high_resolution_clock::now();
    // read the values from the input file and populate the data structure
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int value;
            inputFile >> value;

            // create a Cell object and initialize its value and predefined flag
            Cell cell;
            cell.value = value;
            cell.predefined = (value != 0);

            // add the cell to the Sudoku grid data structure
            puzzleGrid[i][j] = cell;
        }
    }

    inputFile.close();

    const auto end = std::chrono::high_resolution_clock::now();
    _loadTime = end - start;
}
void SudokuPuzzle::buildCandidateLists() {
    candidateValuesConsidered = 0;

    // iterate through each cell in the Sudoku grid
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            // check if the cell is empty
            if (puzzleGrid[row][col].value == 0) {
                // initialize the candidate list of the cell with numbers 1 to 9
                puzzleGrid[row][col].candidateList = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

                // eliminate values already present in the same row, column, and block
                for (int i = 0; i < 9; ++i) {
                    // check value in the same row
                    const int valueInRow = puzzleGrid[row][i].value;
                    if (valueInRow != 0) {
                        auto& candidates = puzzleGrid[row][col].candidateList;
                        // remove valueInRow from the candidate list of the current cell
                        candidates.erase(std::remove(candidates.begin(), candidates.end(), valueInRow), candidates.end());
                        candidateValuesConsidered++;
                    }

                    // check value in the same column
                    const int valueInCol = puzzleGrid[i][col].value;
                    if (valueInCol != 0) {
                        auto& candidates = puzzleGrid[row][col].candidateList;
                        // Remove valueInCol from the candidate list of the current cell
                        candidates.erase(std::remove(candidates.begin(), candidates.end(), valueInCol), candidates.end());
                        candidateValuesConsidered++;
                    }

                    // check value in the same block
                    const int blockRow = (row / 3) * 3 + (i / 3);
                    const int blockCol = (col / 3) * 3 + (i % 3);
                    const int valueInBlock = puzzleGrid[blockRow][blockCol].value;
                    if (valueInBlock != 0) {
                        auto& candidates = puzzleGrid[row][col].candidateList;
                        // remove valueInBlock from the candidate list of the current cell
                        candidates.erase(std::remove(candidates.begin(), candidates.end(), valueInBlock), candidates.end());
                        candidateValuesConsidered++;
                    }
                }
            }
        }
    }
}
void SudokuPuzzle::identifySingles() {
    bool changesMade = true;

    // continue looping until no changes are made in an iteration
    while (changesMade) {
        changesMade = false;

        // iterate through each cell in the Sudoku grid
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                // check if the cell is empty
                if (puzzleGrid[row][col].value == 0) {
                    // check if the cell has only one candidate left
                    if (puzzleGrid[row][col].candidateList.size() == 1) {
                        // assign the remaining candidate as the value for the cell
                        puzzleGrid[row][col].value = puzzleGrid[row][col].candidateList.front();
                        puzzleGrid[row][col].candidateList.clear();

                        // remove the assigned value from the candidate lists in the same row, column, and block
                        for (int i = 0; i < 9; ++i) {
                            auto& rowCandidates = puzzleGrid[row][i].candidateList;
                            rowCandidates.erase(std::remove(rowCandidates.begin(), rowCandidates.end(), puzzleGrid[row][col].value), rowCandidates.end());

                            auto& colCandidates = puzzleGrid[i][col].candidateList;
                            colCandidates.erase(std::remove(colCandidates.begin(), colCandidates.end(), puzzleGrid[row][col].value), colCandidates.end());

                            const int blockRow = (row / 3) * 3 + (i / 3);
                            const int blockCol = (col / 3) * 3 + (i % 3);
                            auto& blockCandidates = puzzleGrid[blockRow][blockCol].candidateList;
                            blockCandidates.erase(std::remove(blockCandidates.begin(), blockCandidates.end(), puzzleGrid[row][col].value), blockCandidates.end());
                        }

                        // increase the counter for solved cells
                        cellsSolved++;

                        // changes were made in this iteration
                        changesMade = true;
                    }
                }
            }
        }
    }
}

void SudokuPuzzle::solve() {
    const auto start = std::chrono::high_resolution_clock::now();

    // initialize variables
    cellsSolved = 0;
    passes = 0;
    candidateValuesConsidered = 0;
    bool solved = false;

    // continue solving until the puzzle is solved
    while (!solved) {
        solved = true;

        // increment the number of passes
        passes++;

        // build candidate lists and identify singles
        buildCandidateLists();
        identifySingles();

        // check if the puzzle is solved
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (puzzleGrid[row][col].value == 0) {
                    solved = false;

                    // if there are candidate values, increment the counter
                    candidateValuesConsidered += puzzleGrid[row][col].candidateList.size();

                    // check if a cell has only one candidate left
                    if (puzzleGrid[row][col].candidateList.size() == 1) {
                        // assign the remaining candidate as the value for the cell
                        puzzleGrid[row][col].value = puzzleGrid[row][col].candidateList.front();
                        puzzleGrid[row][col].candidateList.clear();
                        cellsSolved++;
                        solved = false; // continue solving until no more cells are solved
                    }
                }
            }
        }

        // update the candidate lists in the triple context of the solved cell
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (puzzleGrid[row][col].value != 0) {
                    const int value = puzzleGrid[row][col].value;

                    // update candidate lists in the same row
                    for (int i = 0; i < 9; ++i) {
                        if (puzzleGrid[row][i].value == 0) {
                            auto& candidates = puzzleGrid[row][i].candidateList;
                            candidates.erase(std::remove(candidates.begin(), candidates.end(), value), candidates.end());
                        }
                    }

                    // update candidate lists in the same column
                    for (int i = 0; i < 9; ++i) {
                        if (puzzleGrid[i][col].value == 0) {
                            auto& candidates = puzzleGrid[i][col].candidateList;
                            candidates.erase(std::remove(candidates.begin(), candidates.end(), value), candidates.end());
                        }
                    }

                    // update candidate lists in the same block
                    const int blockRow = (row / 3) * 3;
                    const int blockCol = (col / 3) * 3;
                    for (int i = blockRow; i < blockRow + 3; ++i) {
                        for (int j = blockCol; j < blockCol + 3; ++j) {
                            if (puzzleGrid[i][j].value == 0) {
                                auto& candidates = puzzleGrid[i][j].candidateList;
                                candidates.erase(std::remove(candidates.begin(), candidates.end(), value), candidates.end());
                            }
                        }
                    }
                }
            }
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    _solveTime = end - start;
}




     
// this is an example of how you may output the solved puzzle
void SudokuPuzzle::output(const char filenameOut[]) const {
    std::ofstream outputFile(filenameOut);

    if (outputFile.is_open()) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                outputFile << puzzleGrid[row][col].value << " ";
            }
            outputFile << "\n";
        }

        outputFile.close();
        std::cout << "Solution written to file: " << filenameOut << std::endl;
    }
    else {
        std::cerr << "Unable to open output file: " << filenameOut << std::endl;
    }
}

