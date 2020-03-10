#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int numprocs, rank;
	char message[100];
	MPI_Status status;
	int tag = 42;
	
	MPI_Init(0, 0);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	/* if(rank == 0) {
		MPI_Send(&tag, 1, MPI_INT, (rank + 1) % numprocs, 0, MPI_COMM_WORLD);
		MPI_Recv(&tag, 1, MPI_INT, numprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Proces %d otrzymal %d od procesu %d\n", rank, tag, numprocs - 1);
	} else {
		MPI_Recv(&tag, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Proces %d otrzymal %d od procesu %d\n", rank, tag, rank - 1);
		MPI_Send(&tag, 1, MPI_INT, (rank + 1) % numprocs, 0, MPI_COMM_WORLD);
	} */
	
	if(rank != 0) {
		MPI_Recv(&tag, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Proces %d otrzymal %d od procesu %d\n", rank, tag, rank - 1);
	}
	
	MPI_Send(&tag, 1, MPI_INT, (rank + 1) % numprocs, 0, MPI_COMM_WORLD);
	
	if(rank == 0) {
		MPI_Recv(&tag, 1, MPI_INT, numprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Proces %d otrzymal %d od procesu %d\n", rank, tag, numprocs - 1);
	}
	
	MPI_Finalize();
}
