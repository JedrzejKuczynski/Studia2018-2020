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
    int show_result = 0;
    int range = 0;

    bool *prime_numbers;

    // Wczytanie wartości

    if(argc != 4) {
        printf("Nie podales argumentow! Przykladowe uruchomienie programu: ./dzielenie 10 49 1.\n M - poczatek zakresu, N - koniec zakresu. 0/1 - czy pokazac wynik\n");
        return 0;
    } else {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
        show_result = atoi(argv[3]);
        range = n - m + 1;

        if(m > n) {
            printf("Przedzial <%d, %d> nie istnieje! Moze pomyliles kolejnosc liczb?\n", m, n);
            return 0;
        } else if(m < 2) { // Takie ulatwienie
            printf("Podaj liczbe wieksza badz rowna 2 jako pierwsza liczbe!\n");
            return 0;
        }

        prime_numbers = (bool*)malloc(range * sizeof(prime_numbers));
    }

    for(int i = 0; i < range; i++)
        prime_numbers[i] = false;

    clock_t clock_tstart = clock();
    double start = omp_get_wtime();

    #pragma omp parallel num_threads(num_procs)
    {

        #pragma omp for schedule(dynamic)   // Trza ustalić testowane przedziały
            for(int i = m; i <= n; i++) {
                int root = (int)sqrt(i);

                if(root >= 2) {
                    for(int j = 2; j <= root; j++) {
                        if(i % j == 0) {
                            prime_numbers[i - m] = true;
                            break;
                        }
                    }
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

        for(int i = 0; i < range; i++)
            if(!prime_numbers[i]) {
                printf("%d ", i + m);
                counter++;

                if(counter % 10 == 0)
                    printf("\n");
            }
        printf("\n\n");
        printf("W zakresie od %d do %d znaleziono %d liczb pierwszych.\n\n", m, n, counter);
    }
}
