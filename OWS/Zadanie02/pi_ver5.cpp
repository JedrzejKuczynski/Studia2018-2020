#include <stdio.h>
#include <time.h>
#include <omp.h>


long long num_steps = 1000000000;
double step;
int num_procs = 16;

int main(int argc, char* argv[])
{
    clock_t start, stop;
    double x, pi, sum = 0.0;
    double start1, stop1;
    int i;

    step = 1. / (double)num_steps;

    start = clock();
    start1 = omp_get_wtime();

    #pragma omp parallel num_threads(num_procs) private(i, x)  // Na razie 4 - liczba fizycznych
    {
        #pragma omp for reduction(+:sum)
            for (i = 0; i < num_steps; i++) {
                x = (i + .5) * step;
                sum += 4.0 / (1. + x*x);
            }
    }

    pi = sum * step;

    stop = clock();
    stop1 = omp_get_wtime();

    printf("Wartosc liczby PI wynosi %15.12f\n", pi);
    printf("Wykorzystany czas procesorow wynosi %f sekund\n", ((double)(stop - start)/(double)CLOCKS_PER_SEC));
    printf("Czas przetwarzania wynosi %f sekund\n", stop1 - start1);

    return 0;
}