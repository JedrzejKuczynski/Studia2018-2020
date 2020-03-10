#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int numprocs, rank;
	int data_received = 42;
	
	MPI_Init(0, 0);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int left_neighbour = (rank - 1 + numprocs) % numprocs;
	int right_neighbour = (rank + 1) % numprocs;
	
	MPI_Request reqSend, reqRecv;
	MPI_Status statSend, statRecv;
	
	MPI_Irecv(&data_received, 1, MPI_INT, right_neighbour, 0, MPI_COMM_WORLD, &reqRecv);
	MPI_Isend(&rank, 1, MPI_INT, left_neighbour, 0, MPI_COMM_WORLD, &reqSend);
	MPI_Wait(&reqRecv, &statRecv);
	printf("Proces %d received from right neighbour %d\n", rank, data_received);
	
	MPI_Finalize();
}
