#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
	int numprocs, rank, namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(0, 0);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
	printf("Proces nr %d na komputerze %s - jeden z %d procesow\n", rank, processor_name, numprocs);
	MPI_Finalize();
}
