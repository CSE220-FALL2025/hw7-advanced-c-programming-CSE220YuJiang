#include "unit_tests.h"

TestSuite(student_tests, .timeout=TEST_TIMEOUT); 

Test(student_tests, add_2x2_small, .description = "add two 2x2 matrices (very basic)")
{
    unsigned int rowsA = 2;
    unsigned int colsA = 2;
    int valuesA[4];
    valuesA[0] = 1;
    valuesA[1] = 2;
    valuesA[2] = 3;
    valuesA[3] = 4;
    matrix_sf *A = copy_matrix(rowsA, colsA, valuesA);
    A->name = 'A';

    unsigned int rowsB = 2;
    unsigned int colsB = 2;
    int valuesB[4];
    valuesB[0] = 10;
    valuesB[1] = 20;
    valuesB[2] = 30;
    valuesB[3] = 40;
    matrix_sf *B = copy_matrix(rowsB, colsB, valuesB);
    B->name = 'B';

    matrix_sf *C = add_mats_sf(A, B);

    cr_expect_eq(C->num_rows, 2, "rows should be 2");
    cr_expect_eq(C->num_cols, 2, "cols should be 2");

    cr_expect_eq(C->values[0], 11, "C[0] should be 11");
    cr_expect_eq(C->values[1], 22, "C[1] should be 22");
    cr_expect_eq(C->values[2], 33, "C[2] should be 33");
    cr_expect_eq(C->values[3], 44, "C[3] should be 44");

    free(A);
    free(B);
    free(C);
}
