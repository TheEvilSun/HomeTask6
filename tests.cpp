#include <catch2/catch_test_macros.hpp>
#include "Matrix/SparseMatrix.h"

TEST_CASE("Matrix test") {
    Matrix::SparseMatrix<int, -1> matrix;

    matrix[7][0] = 78;
    matrix[2][10000] = 1047;
    ((matrix[100][52] = 756) = 0) = 258;

    REQUIRE(matrix.size() == 3);

    REQUIRE(matrix[7][0].value() == 78);
    REQUIRE(matrix[7][0].row() == 7);
    REQUIRE(matrix[7][0].column() == 0);

    REQUIRE(matrix[2][10000].value() == 1047);
    REQUIRE(matrix[2][10000].row() == 2);
    REQUIRE(matrix[2][10000].column() == 10000);

    REQUIRE(matrix[100][52].value() == 258);
    REQUIRE(matrix[100][52].row() == 100);
    REQUIRE(matrix[100][52].column() == 52);

    for(const auto& element : matrix) {
        if(element.row() == 7 && element.column() == 0) {
            REQUIRE(element.value() == 78);
        }
        else if(element.row() == 2 && element.column() == 10000) {
            REQUIRE(element.value() == 1047);
        }
        else if(element.row() == 100 && element.column() == 52) {
            REQUIRE(element.value() == 258);
        }
    }

    matrix[2][10000] = -1;

    REQUIRE(matrix.size() == 2);
}
