#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>


long long num_steps = 1000000000;
double step;
int num_procs = 2;
int length = 100;

int main(int argc, char* argv[])
{
    clock_t start, stop;
    double x, pi;
    volatile double sum[length];
    double start1, stop1;
    int i, id, word, s, jj;
    FILE* results;

    cpu_set_t cpuset;
    pthread_t thread;

    step = 1. / (double)num_steps;

    for(i = 0; i < length; i++)
        sum[i] = 0;

    results = fopen("eksperyment.txt", "w");

    for(int it = 0; it < (length - 1); it++) {
        start = clock();
        start1 = omp_get_wtime();

        #pragma omp parallel num_threads(num_procs) private(i, x, id, word, cpuset, thread)
        {
            id = omp_get_thread_num();

            thread = pthread_self();

            CPU_ZERO(&cpuset);
            CPU_SET(id, &cpuset);

            s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
            s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

            // printf("USTAWIONE AFFINITY: %d\n", s);

            /*
            for(jj = 0; jj < sizeof(cpu_set_t); jj++)
                if(CPU_ISSET(jj, &cpuset))
                    printf("Ustalona maska powinowactwa dla procesu %d wynosi %d\n", id, jj);
            */

            if(id == 0)
                word = it;
            else
                word = it + 1;

            // printf("ID PROCESU: %d; SLOWO: %d\n", id, word);

            #pragma omp for
                for (i = 0; i < num_steps; i++) {
                    x = (i + .5) * step;
                    sum[word] += 4.0 / (1. + x*x);
                }
        }

        for(i = it; i <= (it + 1); i++) {
            pi += sum[i] * step;
            sum[i] = 0;
        }

        stop = clock();
        stop1 = omp_get_wtime();

        printf("\n\nWartosc liczby PI wynosi %15.12f\n", pi);
        printf("Wykorzystany czas procesorow wynosi %f sekund\n", ((double)(stop - start)/(double)CLOCKS_PER_SEC));
        printf("Czas przetwarzania wynosi %f sekund\n\n", stop1 - start1);

        fprintf(results, "%f %f\n", ((double)(stop - start)/(double)CLOCKS_PER_SEC), stop1 - start1);

        pi = 0;

    }

    fclose(results);
    return 0;
}