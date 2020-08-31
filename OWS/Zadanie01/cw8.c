#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <limits.h>


// Funkcja majaca na celu znalezienie procesu, ktory przechowuje wynik

int find_index_of_minimum(int test_array[], int size){
    int min_index = -1;
    int min = INT_MAX;

    for(int i = 0; i < size; i++)
        if(test_array[i] < min) {
            min = test_array[i];
            min_index = i;
        }

    return min_index;
}


int test_array[5] = {5, 3, 1, 7, 3};

int main(int argc, char* argv[]) {
    int numprocs, rank, min_index;
    int i, j, w, index = 0;
    int result[5];
    MPI_Comm row_com, col_com;

    MPI_Init(0, 0);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    min_index = find_index_of_minimum(test_array, sizeof(test_array) / sizeof(int));

    // Tworzenie "struktury" kraty NxN procesow, gdzie N to rozmiar tablicy

    i = rank / (sizeof(test_array) / sizeof(int));
    j = rank % (sizeof(test_array) / sizeof(int));

    // Kazdy wiersz "i" procesow zajmuje sie jedna liczba w tablicy (znajdujacej sie pod indeksem "i")
    // i zlicza, ile liczb jest od niej mniejszych (chodzac po kolumnach "j") poprzez okreslenie wartosci zmiennej "w",
    // ktora dalej bedzie sumowana w ramach wiersza "i" procesow
    // Drugi warunek zapewnia oryginalna kolejnosc liczb sobie rownych

    if((test_array[i] > test_array[j]) || (test_array[i] == test_array[j] && (i > j)))
        w = 1;
    else
        w = 0;

    MPI_Comm_split(MPI_COMM_WORLD, i, j, &row_com);  // Podzial komunikatora na N komunikatorow w ramach wiersza.
    												 // Procesy o tym samym "i" naleza do tego samego komunikatora
    												 // Nowa ranga procesu okreslana kolumna "j"

    MPI_Reduce(&w, &index, 1, MPI_INT, MPI_SUM, 0, row_com);  // Nastepuje wyzej wspomniane sumowanie.
    														  // Wynik przechowywany jest w pierwszej kolumnie wierszy (procesie o randze 0)

    MPI_Comm_split(MPI_COMM_WORLD, j, index, &col_com);  // Kolejny podzial, tym razem w ramach kolumn.
    													 // Nowa ranga procesu jest zliczona suma, co pozwoli na
    													 // okreslenie pozycji liczby w posortowanej tablicy

    // Ponizej nastepuje zebranie wynikow w procesie o randze 0 z poszczegolnych procesow w kolumnie 0.
    // Jako ze kazdy z procesow w komunikatorze "col_com", w ramach jednej wartosci "j", znajduje sie w innym wierszu "i"
    // a jego ranga w ramach komunikatora zalezy od obliczonej sumy
    // To proces 0 w ramach komunikatora bedzie odbieral
    // liczby znajdujace sie pod indeksem "i" w oryginalnej tablicy
    // w kolejnosci posortowanej

    if(j == 0) {
        MPI_Gather(&test_array[i], 1, MPI_INT, result, 1, MPI_INT, 0, col_com);
    }

    MPI_Finalize();

    if(rank == min_index * (sizeof(result) / sizeof(int))) {
        for(int idx = 0; idx < sizeof(result) / sizeof(int); idx++)
	       printf("%d, ", result[idx]);
        printf("\n");
    }
}
