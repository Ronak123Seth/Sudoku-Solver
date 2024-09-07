# Sudoku-Solver
Sudoku Solver is a C++ program designed to solve Sudoku puzzles using a backtracking algorithm. The program can accept Sudoku puzzles from manual input, a file, or generate a random puzzle for solving. It provides a user-friendly interface for input and displays the solved puzzle with validation checks.

# Main Classes and Functions
1) Grid Class: Handles the initialization and display of the Sudoku grid.
2) is_safe Function: Checks if placing a number at a specific position is valid.
3) solve_sudoku Function: Recursively solves the puzzle using backtracking.
4) solve Function: Wraps solve_sudoku with error handling.
5) is_valid_solution Function: Validates the solved Sudoku grid.
6) generate_puzzle Function: Generates a random Sudoku puzzle.
7) input_from_user Function: Takes input directly from the user.
8) input_from_file Function: Reads a Sudoku puzzle from a file.
