#include <stdio.h>
#include <string.h>
#include <mpi.h>

int isPalindrome(char *word) {
    int len = strlen(word);
    for (int i = 0; i < len / 2; i++) {
        if (word[i] != word[len - i - 1]) {
            return 0; // No es palíndromo
        }
    }
    return 1; // Es palíndromo
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char *word = "radar"; // Palabra a verificar

    // División de la palabra entre los procesos
    int localResult = isPalindrome(word + rank);
    int globalResult;
    MPI_Allreduce(&localResult, &globalResult, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);

    if (rank == 0) {
        if (globalResult) {
            printf("La palabra \"%s\" es un palíndromo.\n", word);
        } else {
            printf("La palabra \"%s\" no es un palíndromo.\n", word);
        }
    }

    MPI_Finalize();
    return 0;
}
