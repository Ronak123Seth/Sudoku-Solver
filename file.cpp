#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <fstream>

using namespace std;

// A class representing the Sudoku Grid
class Grid {
public:
    vector<vector<int>> grid;

    // Constructor to initialize the Grid object
    Grid(vector<vector<int>> values) {
        if (values.size() != 9 || values[0].size() != 9) {
            throw invalid_argument("Invalid grid size, must be 9x9.");
        }
        grid = values;
    }

    // Method to display the grid
    void display() {
        for (const auto &row : grid) {
            for (int num : row) {
                cout << num << " ";
            }
            cout << endl;
        }
    }
};

// Helper function to check if a number can be placed in the position (row, col)
bool is_safe(const Grid &grid, int row, int col, int num) {
    // Ensure the number is between 1 and 9
    if (num < 1 || num > 9) return false;

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 9; i++) {
        // Check the row
        if (grid.grid[row][i] == num) return false;
        // Check the column
        if (grid.grid[i][col] == num) return false;

        // Check the 3x3 subgrid
        if (grid.grid[startRow + i / 3][startCol + i % 3] == num) return false;
    }
    
    return true;
}


// Function to solve the Sudoku puzzle using backtracking
bool solve_sudoku(Grid *grid) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (grid->grid[row][col] == 0) {  // Empty cell found
                for (int num = 1; num <= 9; num++) {
                    if (is_safe(*grid, row, col, num)) {
                        grid->grid[row][col] = num;
                        if (solve_sudoku(grid)) return true;  // Recursive call
                        grid->grid[row][col] = 0;  // Backtrack
                    }
                }
                return false;  // No valid number found
            }
        }
    }
    return true;  // Puzzle solved
}

// Function to solve the Sudoku puzzle
void solve(Grid *grid) {
    if (!solve_sudoku(grid)) {
        throw logic_error("Sudoku puzzle cannot be solved.");
    }
}

// Function to check if the provided grid is a valid solved Sudoku
bool is_valid_solution(Grid &grid) {
    // Check if numbers are between 0 and 9
    for (const auto &row : grid.grid) {
        for (int num : row) {
            if (num < 0 || num > 9) return false;
        }
    }

    // Check for duplicates in rows, columns, and 3x3 subgrids
    for (int i = 0; i < 9; i++) {
        vector<bool> row_check(9, false), col_check(9, false), subgrid_check(9, false);

        for (int j = 0; j < 9; j++) {
            int num_row = grid.grid[i][j];
            int num_col = grid.grid[j][i];

            if (num_row != 0) {
                if (row_check[num_row - 1]) return false;
                row_check[num_row - 1] = true;
            }

            if (num_col != 0) {
                if (col_check[num_col - 1]) return false;
                col_check[num_col - 1] = true;
            }

            // Check 3x3 subgrid
            int subgrid_row = 3 * (i / 3) + j / 3;
            int subgrid_col = 3 * (i % 3) + j % 3;
            int num_subgrid = grid.grid[subgrid_row][subgrid_col];

            if (num_subgrid != 0) {
                if (subgrid_check[num_subgrid - 1]) return false;
                subgrid_check[num_subgrid - 1] = true;
            }
        }
    }

    return true;
}


// Function to generate a Sudoku puzzle (random generation for simplicity)
Grid generate_puzzle() {
    vector<vector<int>> puzzle(9, vector<int>(9, 0));
    Grid tempGrid(puzzle); // Create a Grid object once and reuse it

    // Randomly filling some cells for demonstration
    srand(time(0));
    for (int i = 0; i < 20; i++) {
        int row = rand() % 9;
        int col = rand() % 9;
        int num = (rand() % 9) + 1;

        // Use the existing Grid object to check if the number can be placed
        if (is_safe(tempGrid, row, col, num)) {
            tempGrid.grid[row][col] = num;
        }
    }

    return tempGrid; // Return the populated Grid object
}

// Function to input Sudoku from the user
Grid input_from_user() {
    vector<vector<int>> values(9, vector<int>(9));
    cout << "Enter the Sudoku grid (use 0 for empty cells):\n";
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cin >> values[i][j];
        }
    }
    return Grid(values);
}

// Function to input Sudoku from a file
Grid input_from_file(const string &filename) {
    vector<vector<int>> values(9, vector<int>(9));
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Unable to open file.");
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            file >> values[i][j];
            if (file.fail()) {
                throw runtime_error("Invalid file format.");
            }
        }
    }
    file.close();
    return Grid(values);
}

// Main function to demonstrate the functionality
int main() {
    int choice;
    Grid puzzle(vector<vector<int>>(9, vector<int>(9, 0)));  // Default initialization
    cout << "Sudoku Solver\n";
    cout << "1. Input Sudoku manually\n";
    cout << "2. Input Sudoku from file\n";
    cout << "3. Generate random Sudoku puzzle\n";
    cout << "Enter your choice (1-3): ";
    cin >> choice;

    try {
        if (choice == 1) {
            puzzle = input_from_user();
            cout << "\nInput Puzzle:\n";
            puzzle.display();
        } else if (choice == 2) {
            string filename;
            cout << "Enter the filename: ";
            cin >> filename;
            puzzle = input_from_file(filename);
            cout << "\nInput Puzzle:\n";
            puzzle.display();
        } else if (choice == 3) {
            puzzle = generate_puzzle();
            cout << "Generated Puzzle:\n";
            puzzle.display();
        } else {
            cout << "Invalid choice!" << endl;
            return 1;
        }

        // Solve the puzzle
        solve(&puzzle);
        cout << "\nSolved Puzzle:\n";
        puzzle.display();

        // Check if the solution is valid
        if (is_valid_solution(puzzle)) {
            cout << "\nThe solution is valid." << endl;
        } else {
            cout << "\nThe solution is not valid." << endl;
        }
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
