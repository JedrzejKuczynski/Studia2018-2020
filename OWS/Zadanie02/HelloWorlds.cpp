#include <stdio.h>
#include <omp.h>
#include <time.h>

int main() {
		int i;
        // omp_set_num_threads(5);

        printf("Hello World\n");

        clock_t start = clock();
        double start1 = omp_get_wtime();

        // 4 Hello World; Intel(R) Core(TM) i5-4690 CPU @ 3.50GHz - 4 rdzeniowy, 4 wątkowy
        #pragma omp parallel num_threads(4)
        {
            int i;
            #pragma omp for
        		for(i = 0; i < 6; i++)
        			printf("Iter:%d wykonal watek %d\n", i, omp_get_thread_num());
        }

        clock_t stop = clock();
        double stop1 = omp_get_wtime();

        printf("Wykorzystany czas procesorow wynosi %f sekund\n", ((double)(stop - start)/(double)CLOCKS_PER_SEC));
        printf("Czas przetwarzania wynosi %f sekund\n", stop1 - start1);

		printf("Goodbye World\n");
}