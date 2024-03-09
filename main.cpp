#include "Matrix/SparseMatrix.h"
#include <iostream>

int main(int argc, char *argv[]) {
    Matrix::SparseMatrix<int, 0> matrix;
    size_t n = 10;
    for(size_t i = 0; i < n; i++) {
        matrix[i][i] = i;
        matrix[i][n - 1 - i] = n - 1 - i;
    }

    std::cout << "Matrix fragment from [1, 1] to [8, 8]:" << std::endl;
    for(size_t i = 1; i < n - 1; i++) {
        for(size_t j = 1; j < n - 1; j++) {
            std::cout << matrix[i][j].value() << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Total busy cells: " << matrix.size() << std::endl << "All busy cells:" << std::endl;

    for(const auto& element : matrix) {
        std::cout << "row: " << element.row() << " column: " << element.column() << " value: " << element.value() << std::endl;
    }
}
