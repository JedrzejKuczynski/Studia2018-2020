#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <limits.h>


int find_index_of_minimum(int test_array[], int size){
    int min_index = -1;
    int min = INT_MAX;

    for(int i = 0; i < size; i++)
        if(test_array[i] < min) {
            min = test_array[i];
            min_index = i;
        }

    return min_index;
}


int test_array[5] = {5, 3, 1, 7, 3};

int main(int argc, char* argv[]) {
    int numprocs, rank, min_index;
    int i, j, w, index = 0;
    int result[5];
    MPI_Comm row_com, col_com;

    MPI_Init(0, 0);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    min_index = find_index_of_minimum(test_array, sizeof(test_array) / sizeof(int));

    i = rank / (sizeof(test_array) / sizeof(int));
    j = rank % (sizeof(test_array) / sizeof(int));

    if((test_array[i] > test_array[j]) || (test_array[i] == test_array[j] && (i > j)))
        w = 1;
    else
        w = 0;

    MPI_Comm_split(MPI_COMM_WORLD, i, j, &row_com);

    MPI_Reduce(&w, &index, 1, MPI_INT, MPI_SUM, 0, row_com);

    MPI_Comm_split(MPI_COMM_WORLD, j, index, &col_com);

    if(j == 0) {
        MPI_Gather(&test_array[i], 1, MPI_INT, result, 1, MPI_INT, 0, col_com);
    }

    MPI_Finalize();

    if(rank == min_index * (sizeof(result) / sizeof(int))) {
        for(int idx = 0; idx < sizeof(result) / sizeof(int); idx++)
	       printf("%d, ", result[idx]);
        printf("\n");
    }
}
