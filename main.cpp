#include "SudokuPuzzle.h"

// You should not need to change any code in this file

int main(const int const argc, char **) {

	{
		SudokuPuzzle puzzle;
		puzzle.load("sudoku_puzzle.txt");
		puzzle.solve();
		puzzle.output("sudoku_solution.txt");
	}

	return 0;
}
