#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>


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
        }

        prime_numbers = (bool*)malloc(range*sizeof(prime_numbers));
    }

    for(int i = m; i <= n; i++) {
        int root = (int)sqrt(i);

        if(root >= 2) {

            // printf("Dla liczby %d pierwiastek wynosi %d\n", i, root);

            for(int j = 2; j <= root; j++) {
                if(i % j == 0) {
                    // printf("Dziele %d przez %d\n", i, j);
                    prime_numbers[i - m] = true;
                    break;
                }
            }
        }
    }

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
