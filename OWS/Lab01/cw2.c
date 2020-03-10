#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int numprocs, rank;
	char message[100];
	MPI_Status status;
	
	MPI_Init(0, 0);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(rank == 0) {
		for(int i = 1; i < numprocs; i++) {
			MPI_Recv(message, 100, MPI_CHAR, i, i, MPI_COMM_WORLD, &status);
			printf("%s\n", message);
		}
	} else {
		snprintf(message, 100, "Pozdrowienia od procesu %d z %d\n", rank, numprocs);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
}
