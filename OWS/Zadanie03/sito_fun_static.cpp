#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>


int num_procs = 4;

int main(int argc, char* argv[]) {

    int m = 0;
    int n = 0;
    int n_root = 0;
    int range = 0;
    int show_result = 0;

    int i, j;

    bool *prime_check;
    bool *primes_to_root;
    bool *primes_in_range;

    // Wczytanie wartości

    if(argc != 4) {
        printf("Nie podales argumentow! Przykladowe uruchomienie programu: ./sito 10 49 1.\n M - poczatek zakresu, N - koniec zakresu. 0/1 - czy pokazac wynik\n");
        return 0;
    } else {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
        show_result = atoi(argv[3]);

        if(m > n) {
            printf("Przedzial <%d, %d> nie istnieje! Moze pomyliles kolejnosc liczb?\n", m, n);
            return 0;
        } else if(m < 2) { // Takie ulatwienie
            printf("Podaj liczbe wieksza badz rowna 2 jako pierwsza liczbe!\n");
            return 0;
        }
    }

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

    clock_t clock_tstart = clock();
    double start = omp_get_wtime();

    #pragma omp parallel num_threads(num_procs) private(i, j) shared(primes_to_root, primes_in_range)
    {

        // int thread_id = omp_get_thread_num();

        #pragma omp for schedule(static)  // Trza ustalić testowane przedziały
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

    printf("Wykorzystany czas procesorow wynosi %f sekund\n", ((double)(clock_tstop - clock_tstart)/(double)CLOCKS_PER_SEC));
    printf("Czas przetwarzania wynosi %f sekund\n\n", stop - start);

    if(show_result == 1) {

        int counter = 0;

        printf("\n");

        for(int i = m; i <= n; i++)
            if(!primes_in_range[i - m]) {
                printf("%d ", i);
                counter++;

                if(counter % 10 == 0)
                    printf("\n");
            }
        printf("\n\n");
        printf("W zakresie od %d do %d znaleziono %d liczb pierwszych.\n\n", m, n, counter);
    }
}
