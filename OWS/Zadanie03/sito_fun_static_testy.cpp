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

    printf("ZACZALEM TESTY FUNKCYJNEGO STATIC\n");

    FILE *results = fopen("Testy/sito_fun_static.txt", "a");

    for(int np = 0; np < 2; np++)
        for(int mn = 0; mn < 3; mn++) {

            int m = m_n[mn][0];
            int n = m_n[mn][1];
            int n_root = 0;
            int range = 0;

            int i, j;

            bool *prime_check;
            bool *primes_to_root;
            bool *primes_in_range;

            n_root = int(sqrt(n));
            range = n - m + 1;

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

                        #pragma omp for schedule(static, params[par])  // Trza ustalić testowane przedziały
                            for(int p = 2; p < n_root + 1; p++) {

                                if(primes_to_root[p] == true) {

                                    // printf("PROCES %d OTRZYMAL LICZBE PIERWSZA %d\n\n", thread_id, p);

                                    int lowest = floor(m / p) * p;

                                    if(lowest < m)
                                        lowest += p;

                                    if(lowest == p)
                                        lowest += p;

                                    for(j = lowest; j <= n; j += p)
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
