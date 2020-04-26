#include <stdio.h>
#include <time.h>
#include <omp.h>


long long num_steps = 1000000000;
double step;
int num_procs = 4;

int main(int argc, char* argv[])
{
    clock_t start, stop;
    double x, pi, sum[num_procs];
    double start1, stop1;
    int i, id;

    step = 1. / (double)num_steps;

    start = clock();
    start1 = omp_get_wtime();

    #pragma omp parallel num_threads(num_procs) private(i, x, id) // Na razie 4 - liczba fizycznych
    {
        id = omp_get_thread_num();

        #pragma omp for
            for (i = 0; i < num_steps; i++) {
                x = (i + .5) * step;
                sum[id] += 4.0 / (1. + x*x);
            }
    }

    for(int i = 0; i < num_procs; i++)
        pi += sum[i] * step;

    stop = clock();
    stop1 = omp_get_wtime();

    printf("Wartosc liczby PI wynosi %15.12f\n", pi);
    printf("Wykorzystany czas procesorow wynosi %f sekund\n", ((double)(stop - start)/(double)CLOCKS_PER_SEC));
    printf("Czas przetwarzania wynosi %f sekund\n", stop1 - start1);

    return 0;
}