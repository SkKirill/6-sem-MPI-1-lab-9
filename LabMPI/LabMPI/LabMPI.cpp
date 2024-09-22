#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <locale>
#include <iostream>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size % 2 != 0) { 
        fprintf(stderr, "Количество процессов должно быть четным\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int m = 8; 
    int* X = (int*)malloc(m * sizeof(int)); 

    std::set<int> uniqueNumbers;

    srand(rank);

    for (int i = 0; i < m; i++) {
        while (true) {
            int current = rand() % 30;
            if (uniqueNumbers.insert(current).second) {
                X[i] = current;
                break;
            }
        }
    }

    printf("Process %i: {", rank);
    for (int i = 0; i < m; i++) {
        printf(" %i ", X[i]);
    }
    printf("}\n");

    if (rank % 2 != 0) {
        MPI_Send(X, m, MPI_INT, rank - 1, 0, MPI_COMM_WORLD); 
    }
    else {
        int* X2 = (int*)malloc(m * sizeof(int));
        MPI_Recv(X2, m, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("X%i U X%i = {", rank, rank + 1);
        for (int i = 0; i < m; i++) {
            bool ok = true;
            for (int j = 0; j < m && ok; j++) {
                ok = X[i] != X2[j];
            }
            if (ok) printf(" %i ", X[i]);
        }
        for (int i = 0; i < m; i++) {
            printf(" %i ", X2[i]);
        }
        printf("}\n");
    }

    free(X);
    MPI_Finalize();
    return 0;
}