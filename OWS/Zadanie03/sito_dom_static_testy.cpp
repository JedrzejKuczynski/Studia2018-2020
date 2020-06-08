#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>


int num_procs[2] = {2, 4};
int m_n[3][2] = {
    {2, 1000000000},
    {2, 500000000},
    {500000000, 1000000000}
};
int params[10] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

int main(int argc, char* argv[]) {

    printf("ZACZALEM TESTY DOMENOWEGO STATIC\n\n");

    FILE *results = fopen("Testy/sito_dom_static.txt", "a");
    int show_result = 0;

    for(int np = 0; np < 2; np++)
        for(int mn = 0; mn < 3; mn++) {

            int m = m_n[mn][0];
            int n = m_n[mn][1];
            int n_root = int(sqrt(n));
            int range = n - m + 1;
            int range_root = round(sqrt(range));

            int i, j;

            bool *prime_check;
            bool *primes_to_root;
            bool *primes_in_range;

            prime_check = (bool*)malloc((n_root + 1) * sizeof(prime_check));  // tablica wykreslen do pierwiastka z N
            primes_to_root = (bool*)malloc((n_root + 1) * sizeof(primes_to_root));  // tablica okreslajaca liczby pierwsze do pierwiastka z N
            primes_in_range = (bool*)malloc(range * sizeof(primes_in_range));  // tablica wykreslen dla przedzialu <M, N>

            for(i = 0; i < n_root + 1; i++)
                prime_check[i] = false;

            for(i = 0; i < n_root + 1; i++)
                primes_to_root[i] = false;

            for(i = 0; i < range; i++)
                primes_in_range[i] = false;

            // Sito <2, sqrt(n)>

            for(i = 2; i < n_root + 1; i++)
                if(prime_check[i] == false) {
                    primes_to_root[i] = true;

                    for(j = i; j < n_root + 1; j += i)
                        prime_check[j] = true;
                }

            // ---- TUTAJ POCZĄTEK OMP PARALELL ---- //

            for(int par = 0; par < 10; par++) {
                fprintf(results, "%d,%d,%d,%d\n", num_procs[np], m, n, params[par]);
                for(int it = 0; it < 3; it++) {

                    clock_t clock_tstart = clock();
                    double start = omp_get_wtime();

                    #pragma omp parallel num_threads(num_procs[np]) private(i, j) shared(primes_to_root, primes_in_range)
                    {

                        // int thread_id = omp_get_thread_num();

                        #pragma omp for schedule(static, params[par])
                            for(int p = 0; p < range_root; p++) {

                                int start = p * range_root + m;
                                int end = 0;
                                
                                if(p == range_root - 1)
                                    end = n + 1;
                                else
                                    end = start + range_root;

                                // printf("PROCES %d wita sie! START: %d; END: %d\n\n", thread_id, start, end);

                                for(i = 2; i < n_root + 1; i++)
                                    // printf("PROCES %d aktualne i: %d\n\n", thread_id, i);
                                    if(primes_to_root[i] == true) {
                                        // printf("PROCES %d potwierdza, ze %d jest pierwsza i robi prace\n\n", thread_id, i);
                                        int lowest = floor(start / i) * i;

                                        if(lowest < m)
                                            lowest += i;

                                        if(lowest == i)
                                            lowest += i;

                                        // printf("PROCES %d znalazl %d i raportuje, ze zacznie wykreslanie od %d\n\n", thread_id, i, lowest);

                                        for(j = lowest; j < end; j += i)
                                            if(primes_in_range[j - m] != true)
                                                primes_in_range[j - m] = true;
                                    }
                            }
                    }

                    clock_t clock_tstop = clock();
                    double stop = omp_get_wtime();

                    // printf("Wykorzystany czas procesorow wynosi %f sekund\n", ((double)(clock_tstop - clock_tstart)/(double)CLOCKS_PER_SEC));
                    // printf("Czas przetwarzania wynosi %f sekund\n\n", stop - start);

                    fprintf(results, "%f,%f\t", ((double)(clock_tstop - clock_tstart)/(double)CLOCKS_PER_SEC), stop - start);

                    for(i = 0; i < range; i++)
                        primes_in_range[i] = false;
                }
                fprintf(results, "\n");
            }
        }
    fclose(results);
}
