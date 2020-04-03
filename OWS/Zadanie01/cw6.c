#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <string.h>
#include <math.h>


int main(int argc, char* argv[]) {
    int num_procs, rank;
    int data_received_right = -1;
    int data_received_bottom = -2;

    MPI_Status  statRecv[2];
    MPI_Request reqSend[2], reqRecv[2];


    MPI_Init(0, 0);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int PP = sqrt((double)num_procs);

    int row = rank / PP;
    int col = rank % PP;

    int right = (row * PP) + (col + 1) % PP;
    int bottom = ((row + 1) % PP) * PP + col;
    int top = ((row + PP - 1) % PP) * PP + col;
    int left = (row * PP) + (col - 1 + PP) % PP;

    MPI_Isend(&rank, 1, MPI_INT, left, 42, MPI_COMM_WORLD, reqSend);
    MPI_Isend(&rank, 1, MPI_INT, top, 42, MPI_COMM_WORLD, &reqSend[1]);

    MPI_Irecv(&data_received_right, 1, MPI_INT, right, 42, MPI_COMM_WORLD, reqRecv);
    MPI_Irecv(&data_received_bottom, 1, MPI_INT, bottom, 42, MPI_COMM_WORLD, &reqRecv[1]);

    MPI_Wait(reqSend, statRecv);
    MPI_Wait(&reqSend[1], &statRecv[1]);

    MPI_Barrier(MPI_COMM_WORLD);

    printf("Rank %d odebrał: od prawego %d, a od dolnego %d!\n\n", rank, data_received_right, data_received_bottom);
    printf("Rank %d wysyła: do lewego sąsiada - %d oraz do górnego sąsiada - %d!\n\n", rank, left, top);

    MPI_Finalize();

}