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
        n_root = int(sqrt(n));
        range = n - m + 1;

        if(m > n) {
            printf("Przedzial <%d, %d> nie istnieje! Moze pomyliles kolejnosc liczb?\n", m, n);
            return 0;
        } else if(m < 2) { // Takie ulatwienie
            printf("Podaj liczbe wieksza badz rowna 2 jako pierwsza liczbe!\n");
            return 0;
        }

        prime_check = (bool*)malloc((n_root + 1) * sizeof(prime_check));  // tablica wykreslen do pierwiastka z N
        primes_to_root = (bool*)malloc((n_root + 1) * sizeof(primes_to_root));  // tablica okreslajaca liczby pierwsze do pierwiastka z N
        primes_in_range = (bool*)malloc(range * sizeof(primes_in_range));  // tablica wykreslen dla przedzialu <M, N>
    }

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

    // Ustalanie podzialu pracy

    int elements_per_thread = range / num_procs;
    int remainder = range % num_procs;

    // ---- TUTAJ POCZĄTEK OMP PARALELL ---- //

    clock_t clock_tstart = clock();
    double start = omp_get_wtime();

    #pragma omp parallel num_threads(num_procs) private(i, j) shared(primes_to_root, primes_in_range)
    {

        int thread_id = omp_get_thread_num();
        int start = (thread_id * elements_per_thread) + fmin(thread_id, remainder) + m;
        int end = ((thread_id + 1) * elements_per_thread) + fmin(thread_id + 1, remainder) - 1 + m;

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

                for(j = lowest; j <= end; j += i)
                    if(primes_in_range[j - m] != true)
                        primes_in_range[j - m] = true;
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
