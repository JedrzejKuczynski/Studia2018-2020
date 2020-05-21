#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>


int main(int argc, char* argv[]) {

    int m = 0;
    int n = 0;
    int n_root = 0;
    int range = 0;
    int show_result = 0;
    bool *prime_check;
    bool *primes_to_root;
    int *primes_in_range;

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
        }

        primes_to_root = (bool*)malloc((n_root + 2)*sizeof(primes_to_root));  // tablica okreslajaca liczby pierwsze do pierwiastka z N
        prime_check = (bool*)malloc((n_root + 2)*sizeof(prime_check));  // tablica wykreslen
        primes_in_range = (int*)malloc(range*sizeof(primes_in_range));

    }

    // Sito <2, sqrt(n)>

    for(int i = 2; i < n_root + 2; i++)
        if(prime_check[i] == false) {
            primes_to_root[i] = true;

            for(int j = i; j < n_root + 2; j += i)
                prime_check[j] = true;
        }

    // Sito <M, N>

    for(int i = 2; i < n_root + 2; i++)
        if(primes_to_root[i] == true) {
            int lowest = floor(m / i) * i;

            if(lowest < m)
                lowest += i;

            if(lowest == i)
                lowest += i;

            for(int j = lowest; j <= n; j += i)
                primes_in_range[j - m] = true;
        }

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
