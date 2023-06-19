#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <stdint.h>

#define BASE 16
#define HEX_DIGITS 16
#define PRECISION 100

void calc_term(uint64_t k, double* result) {
    double a = 1.0 / pow(BASE, k);
    double b = 4.0 / (8 * k + 1);
    double c = 2.0 / (8 * k + 4);
    double d = 1.0 / (8 * k + 5);
    double e = 1.0 / (8 * k + 6);
    *result = a * (b - c - d - e);
}

void calculate_pi(uint64_t start, uint64_t end, double* sum) {
    double term;
    for (uint64_t k = start; k <= end; k++) {
        calc_term(k, &term);
        *sum += term;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    uint64_t n = PRECISION / size;  // Número de términos a calcular por proceso
    uint64_t start = rank * n + 1;  // Índice de inicio
    uint64_t end = start + n - 1;   // Índice de fin

    double local_sum = 0.0;
    calculate_pi(start, end, &local_sum);

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 1 / global_sum;
        printf("Pi: %.16f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
