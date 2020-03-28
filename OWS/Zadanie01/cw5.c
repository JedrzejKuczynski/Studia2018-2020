#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <omp.h>

/* 
 * step = 1./(double)num_steps;
 	for (i=0; i<num_steps; i++)
	{
		x = (i + .5)*step;
		sum = sum + 4.0/(1.+ x*x);
	}
	*/
	
int num_steps = 1000000000;
double step;

int main(int argc, char* argv[]) {
	int num_procs, rank;
	double x, my_pi, pi;
	
	MPI_Init(0, 0);
	
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	/*
	if(rank == 0) {
		fprintf(stdout, "Podaj liczbe krokow: (0 konczy prace): ");
		fflush(stdout);
		if(scanf("%d", &num_steps) != 1) {
			fprintf(stdout, "Blad podczas wprowadzania wartosci! Koniec pracy\n");
			num_steps = 0;
		}
	}
	*/
	
	double start_time = 0.0, end_time;

	start_time = MPI_Wtime();
	
	MPI_Bcast(&num_steps, 1, MPI_INT, 0, MPI_COMM_WORLD);
	step = 1. / (double)num_steps;
	
	int process_steps = num_steps / num_procs;
	int start = rank * process_steps;
	int end = (rank + 1) * process_steps;
	
	for(start; start < end; start++) {
		x = (start + 0.5) * step;
		my_pi = my_pi + 4 / (1 + x*x);
	}
	
	my_pi = my_pi * step;
	
	MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	end_time = MPI_Wtime();
	
	if(rank == 0){
		fprintf(stdout, "Wartosc liczby pi: %f\n", pi);
		printf("Czas obliczen = %f\n", end_time - start_time);
	}
	
	MPI_Finalize();
}
