#
# @file     gen_test_data.py
# @brief    Generates data to test my matrix.c functions against
# @author   CJ vd Walt (christian@vanderwalts.net)
# @data     20/01/2026
#

import time

import numpy as np

# Constants
TEST_DATA_FILE = "./include/test_data_matrix.h"
HEADER = """/**
 * @file    test_data_matrix.h
 * @brief   generated data to use with test_matrix.c
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    20/01/2026
*/\n"""
TESTDATA = "\n/*** Test Data ***/\n\n"
ADDITION = "\n/*** Addition Test Results ***/\n\n"


def write_array(file, arr):
    for num in arr[:-1]:
        file.write(str(num) + ", ")
    file.write(str(arr[-1]))


def main():
    rng = np.random.default_rng()

    with open(TEST_DATA_FILE, "w", encoding="utf-8") as f:
        f.write(HEADER)

        # Get the random arrays
        shape_A = (2, 5)
        shape_B = shape_A
        shape_C = (4, 5)
        data_A = rng.normal(0, 100, shape_A[0] * shape_A[1])
        data_B = rng.normal(0, 100, shape_B[0] * shape_B[1])
        data_C = rng.normal(0, 100, shape_C[0] * shape_C[1])

        # Turn data into a matrix
        mat_A = np.ndarray(shape_A, data_A.dtype, data_A)
        mat_B = np.ndarray(shape_B, data_B.dtype, data_B)
        mat_C = np.ndarray(shape_C, data_C.dtype, data_C)

        # Write data to header file
        f.write(TESTDATA)
        f.write("const int SHAPE_A[] = { ")
        f.write(str(shape_A[0]) + ", " + str(shape_A[1]) + " };\n")
        f.write("const int SHAPE_B[] = { ")
        f.write(str(shape_B[0]) + ", " + str(shape_B[1]) + " };\n")
        f.write("const int SHAPE_C[] = { ")
        f.write(str(shape_C[0]) + ", " + str(shape_C[1]) + " };\n")

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

        # Initialization Results

        # Write addition results

        f.write(ADDITION)


main()
