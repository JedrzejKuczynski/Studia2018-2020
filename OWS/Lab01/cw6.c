#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int numprocs, rank;
	
	MPI_Init(0, 0);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	MPI_Finalize();
	
}
