#
# @file     gen_test_data.py
# @brief    Generates data to test my matrix.c functions against
# @author   CJ vd Walt (christian@vanderwalts.net)
# @data     20/01/2026
#

import time

import numpy as np
import sympy as sp

# Constants
TEST_DATA_FILE = "./include/test_data_matrix.h"
HEADER = """/**
 * @file    test_data_matrix.h
 * @brief   generated data to use with test_matrix.c
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    20/01/2026
*/\n"""
TESTDATA = "\n/*** Test Data ***/\n\n"
INIT = "\n/*** Initialization Test Results ***/\n\n"
ADDITION = "\n/*** Addition Test Results ***/\n\n"
MULTIPLICATION = "\n/*** Multiplication Test Results ***/\n\n"
TRANSPOSE = "\n/*** Transpose Test Results ***/\n\n"
RANK = "\n/*** Rank Results ***/\n\n"
RREF = "\n/*** Reduced Row Echolon Form Test Results ***/\n\n"
INVERSE = "\n/*** Inverse Test Results ***/\n\n"


def write_array(file, arr):
    for num in arr[:-1]:
        file.write(str(num) + ", ")
    file.write(str(arr[-1]))


def main():
    rng = np.random.default_rng()

    with open(TEST_DATA_FILE, "w", encoding="utf-8") as f:
        f.write(HEADER)

        # Get the random arrays
        shape_A = (3, 3)
        shape_B = shape_A
        shape_C = (shape_A[1], 3)
        shape_D = (shape_C[1], shape_A[0])
        data_A = rng.integers(-50, 50, shape_A[0] * shape_A[1])
        data_B = rng.integers(-50, 50, shape_B[0] * shape_B[1])
        data_C = rng.integers(-50, 50, shape_C[0] * shape_C[1])
        data_D = rng.integers(-50, 50, shape_D[0] * shape_D[1])

        # Turn data into a matrix
        stime = time.perf_counter()
        mat_A = np.ndarray(shape_A, data_A.dtype, data_A)
        mat_B = np.ndarray(shape_B, data_B.dtype, data_B)
        mat_C = np.ndarray(shape_C, data_C.dtype, data_C)
        mat_D = np.ndarray(shape_D, data_D.dtype, data_D)
        init_t = time.perf_counter() - stime

        # Write data to header file
        f.write(TESTDATA)
        f.write("const int SHAPE_A[] = { ")
        f.write(str(shape_A[0]) + ", " + str(shape_A[1]) + " };\n")
        f.write("const int SHAPE_B[] = { ")
        f.write(str(shape_B[0]) + ", " + str(shape_B[1]) + " };\n")
        f.write("const int SHAPE_C[] = { ")
        f.write(str(shape_C[0]) + ", " + str(shape_C[1]) + " };\n")
        f.write("const int SHAPE_D[] = { ")
        f.write(str(shape_D[0]) + ", " + str(shape_D[1]) + " };\n")

        f.write("\n")

        f.write("const double TEST_DATA_A[] = { ")
        write_array(f, data_A)
        f.write(" };\n")
        f.write("const double TEST_DATA_B[] = { ")
        write_array(f, data_B)
        f.write(" };\n")
        f.write("const double TEST_DATA_C[] = { ")
        write_array(f, data_C)
        f.write(" };\n")
        f.write("const double TEST_DATA_D[] = { ")
        write_array(f, data_D)
        f.write(" };\n")

        f.write("\n")

        # Write Data as Column-Major
        f.write("const double TEST_FDATA_A[] = { ")
        write_array(f, mat_A.flatten("F"))
        f.write(" };\n")
        f.write("const double TEST_FDATA_B[] = { ")
        write_array(f, mat_B.flatten("F"))
        f.write(" };\n")
        f.write("const double TEST_FDATA_C[] = { ")
        write_array(f, mat_C.flatten("F"))
        f.write(" };\n")
        f.write("const double TEST_FDATA_D[] = { ")
        write_array(f, mat_D.flatten("F"))
        f.write(" };\n")

        # Matrix init testing
        f.write(INIT)
        f.write("const double INIT_T = " + str(round(init_t, 11)) + ";\n")

        # Matrix addtion
        stime = time.perf_counter()
        mat_A_plus_B = mat_A + mat_B
        mat_C_plus_C = mat_C + mat_C
        etime = time.perf_counter()
        add_t = etime - stime

        # Write addition results
        f.write(ADDITION)
        f.write("const double MAT_A_P_B[] = { ")
        write_array(f, mat_A_plus_B.flatten())
        f.write(" };\n")
        f.write("const double MAT_C_P_C[] = { ")
        write_array(f, mat_C_plus_C.flatten())
        f.write(" };\n")

        f.write("\n")

        f.write("const double ADD_T = " + str(round(add_t, 12)) + ";\n")

        # Matrix multiplication
        stime = time.perf_counter()
        mat_A_mul_C = np.matmul(mat_A, mat_C)
        mat_B_mul_C = np.matmul(mat_B, mat_C)
        mat_C_mul_D = np.matmul(mat_C, mat_D)
        mat_D_mul_A = np.matmul(mat_D, mat_A)
        mat_D_mul_B = np.matmul(mat_D, mat_B)
        etime = time.perf_counter()
        mult_t = etime - stime

        f.write(MULTIPLICATION)
        f.write("const double MAT_A_M_C[] = { ")
        write_array(f, mat_A_mul_C.flatten())
        f.write(" };\n")
        f.write("const double MAT_B_M_C[] = { ")
        write_array(f, mat_B_mul_C.flatten())
        f.write(" };\n")
        f.write("const double MAT_C_M_D[] = { ")
        write_array(f, mat_C_mul_D.flatten())
        f.write(" };\n")
        f.write("const double MAT_D_M_A[] = { ")
        write_array(f, mat_D_mul_A.flatten())
        f.write(" };\n")
        f.write("const double MAT_D_M_B[] = { ")
        write_array(f, mat_D_mul_B.flatten())
        f.write(" };\n")

        f.write("\n")

        f.write("const double MUL_T = " + str(round(mult_t, 12)) + ";\n")

        # Matrix transpose
        stime = time.perf_counter()
        mat_A_T = mat_A.transpose()
        mat_B_T = mat_B.transpose()
        mat_C_T = mat_C.transpose()
        mat_D_T = mat_D.transpose()
        etime = time.perf_counter()
        t_t = etime - stime

        f.write(TRANSPOSE)
        f.write("const int SHAPE_A_T[] = { ")
        f.write(str(shape_A[1]) + ", " + str(shape_A[0]) + " };\n")
        f.write("const int SHAPE_B_T[] = { ")
        f.write(str(shape_B[1]) + ", " + str(shape_B[0]) + " };\n")
        f.write("const int SHAPE_C_T[] = { ")
        f.write(str(shape_C[1]) + ", " + str(shape_C[0]) + " };\n")
        f.write("const int SHAPE_D_T[] = { ")
        f.write(str(shape_D[1]) + ", " + str(shape_D[0]) + " };\n")

        f.write("\n")

        f.write("const double MAT_A_T[] = { ")
        write_array(f, mat_A_T.flatten())
        f.write(" };\n")
        f.write("const double MAT_B_T[] = { ")
        write_array(f, mat_B_T.flatten())
        f.write(" };\n")
        f.write("const double MAT_C_T[] = { ")
        write_array(f, mat_C_T.flatten())
        f.write(" };\n")
        f.write("const double MAT_D_T[] = { ")
        write_array(f, mat_D_T.flatten())
        f.write(" };\n")

        f.write("\n")

        f.write("const double T_T = " + str(round(t_t, 12)) + ";\n")

        # Matrix ranks and RREF form
        spmat_A = sp.Matrix(mat_A)
        spmat_B = sp.Matrix(mat_B)
        spmat_C = sp.Matrix(mat_C)
        spmat_D = sp.Matrix(mat_D)
        stime = time.perf_counter()
        rref_A, rank_A = spmat_A.rref()
        rref_B, rank_B = spmat_B.rref()
        rref_C, rank_C = spmat_C.rref()
        rref_D, rank_D = spmat_D.rref()
        etime = time.perf_counter()
        rref_t = etime - stime

        f.write("const int RANK_A = " + str(len(rank_A)) + ";\n")
        f.write("const int RANK_B = " + str(len(rank_B)) + ";\n")
        f.write("const int RANK_C = " + str(len(rank_C)) + ";\n")
        f.write("const int RANK_D = " + str(len(rank_D)) + ";\n")

        f.write("\n")

        f.write("const double RREF_A[] = { ")
        write_array(f, sp.flatten(rref_A))
        f.write(" };\n")
        f.write("const double RREF_B[] = { ")
        write_array(f, sp.flatten(rref_B))
        f.write(" };\n")
        f.write("const double RREF_C[] = { ")
        write_array(f, sp.flatten(rref_C))
        f.write(" };\n")
        f.write("const double RREF_D[] = { ")
        write_array(f, sp.flatten(rref_D))
        f.write(" };\n")

        f.write("\n")

        f.write("const double RREF_T = " + str(round(rref_t, 12)) + ";\n")


main()
