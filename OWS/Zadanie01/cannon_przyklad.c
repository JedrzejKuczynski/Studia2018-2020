#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 2000
#define PP 2
#define P 4


// const int n = 2000, PP = 2, P = 4;  // wielkosc mnozonych macierzy, pierwiastek z liczby procesow, liczba procesow

float a[n / PP][n / PP], b[n / PP][n / PP], c[n / PP][n / PP];
float aa[n / PP][n / PP], bb[n / PP][n / PP];
float (*psa)[n / PP], (*psb)[n / PP], (*pra)[n / PP], (*prb)[n / PP];

double startwtime1,startwtime2, endwtime;

int main(int argc, char **argv) {

    FILE *plik;
    FILE *plik_out;

	int my_rank, ncpus;
	int row, col, mod = 0;
	int data_received = -1;
	int tag = 101;
	int koniec;
	
	MPI_Status  statRecv[2];
	MPI_Request reqSend[2], reqRecv[2];

    MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &ncpus); 

    if(my_rank == 0)
        printf("obliczenia metoda Cannona dla tablicy %d x %d elementów \n", n, n);

    if(my_rank == 0)
        startwtime1 = MPI_Wtime();  // czas w sekundach

    // ---- CZYTANIE DANYCH ---- //

    // wczytanie danych przez proces rank = 0
    if(my_rank == 0) {
        plik = fopen("liczby.txt","r");
        if(plik == NULL) {
            printf("Blad otwarcia pliku \"liczby.txt\"\n");
            koniec=1;
            MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Finalize();
            exit(0);
        }else {
            koniec=0;
            MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }else {
        MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if(koniec) {
            MPI_Finalize();
            exit(0);
        }
    }

    if(ncpus != P) {
           if(my_rank == 0)
                printf("wywolano obliczenia iloczynu macierzy metoda cannona na %d procesach - uruchom mpiexec -n %d matrixmult\n", ncpus, P);

        MPI_Finalize();
        exit(0);
    }

    if(my_rank == 0) {
    // w pliku danych musi wystarczyc
    // czyta a do wyslania do innych procesow
    // konieczne przygotowanie danych wejsciowych w postaci 
    // zgodnej ze poczatkowym sposobem dystrybucji szachownicowej

    for(int kk = 1; kk < PP*PP; kk++) {
        for(int i = 0; i < n / PP; i++)
            for(int j = 0; j < n / PP; j++) {
                fscanf(plik,"%f", &a[i][j]);
            }

        MPI_Isend(a, n*n / PP / PP, MPI_FLOAT, kk, tag, MPI_COMM_WORLD, reqSend);

    // test konca komunikacji
    }

    // czyta dla siebie

    for(int i = 0; i < n / PP; i++)
        for(int j = 0; j < n / PP; j++) {
            fscanf(plik,"%f", &a[i][j]);
        }

    // czyta b do wyslania do innych procesow
    // konieczne przygotowanie danych wejsciowych w postaci 
    // zgodnej ze poczatkowym sposobem dystrybucji szachownicowej

    for(int kk = 1; kk < PP*PP; kk++) {  // kolejne identyfikatory procesow
        for(int i = 0; i < n / PP; i++)
            for(int j = 0; j < n / PP; j++) {
                fscanf(plik,"%f", &b[i][j]);
            }

        MPI_Isend(b, n*n / PP / PP, MPI_FLOAT, kk, tag, MPI_COMM_WORLD,reqSend);

    //test konca komunikacji			
    }

    // czyta dla siebie

    for(int i = 0; i < n / PP; i++)
        for(int j = 0; j < n / PP; j++) {
            fscanf(plik,"%f",&b[i][j]);
        }

    fclose(plik);
    }else {
		MPI_Irecv(a, n*n / PP / PP, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, reqRecv);
		// test konca komunikacji
		MPI_Irecv(b, n*n / PP / PP, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &reqRecv[1]);
		// test konca komunikacji
    }

    // ---- KONIEC CZYTANIA DANYCH ---- //

    // przygotowanie tablicy wynikowej

    row = my_rank / PP;
    col = my_rank % PP;

    for(int i = 0; i < n / PP; i++)
        for(int j = 0; j < n / PP; j++) {
            c[i][j] = 0;
        }

    if(my_rank == 0)
        startwtime2 = MPI_Wtime();  //czas w sekundach

    // obliczenia iloczynu macierzy zgodnie z algorytmem Cannona 
    // do uzupelnienia

    pra = aa;
    prb = bb;
    psa = a;
    psb = b;

    for(int kk = 0; k < PP; kk+) { // Iteracja przetwarzania
        for(int i = 0; i < n / PP; i++)
            for(int k = 0; k < n / PP; k++)
                for(int j = 0; j < n / PP; j++)
                    c[i][j] += psa[i][k] * psb[k][j];

        MPI_Irecv(pra, n*n / PP / PP, MPI_FLOAT, prawy, tag, MPI_COMM_WORLD, reqRecv);
        MPI_Irecv(prb, n*n / PP / PP, MPI_FLOAT, dolny, tag, MPI_COMM_WORLD, &reqRecv[1]);
        MPI_Isend(psa, n*n / PP / PP, MPI_FLOAT, górny, tag, MPI_COMM_WORLD, reqSend);
        MPI_Isend(psb, n*n / PP / PP, MPI_FLOAT, lewy, tag, MPI_COMM_WORLD, &reqSend[1]);

        MPI_Wait(reqRecv, statRecv);
        MPI_Wait(&reqRecv[1], &statRecv[1]);

        if(mod = ((mod + 1) % 2)) {
            pra = a;
            prb = b;
            psa = aa;
            psb = bb;
        }else {
            pra = aa;
            prb = bb;
            psa = a;
            psb = b;
        }
    }

    if(my_rank == 0) {
	   endwtime = MPI_Wtime();
       printf("Calkowity czas przetwarzania wynosi %f sekund\n", endwtime - startwtime1);
       printf("Calkowity czas obliczen wynosi %f sekund\n", endwtime - startwtime2);
    }

    // test poprawnosci wyniku - wynik do pliku lub inny sposob

    if(my_rank == 0) {
        plik_out = fopen("wynik.txt", "w");

        if(plik_out == NULL) {
            printf("Blad otwarcia pliku \"wynik.txt\"\n");
            exit(0);
        }

        for(int i = 0; i < n / PP; i++) {
		  for(int j = 0; j < n / PP ; j++)
            fprintf(plik_out, "%6.1f", c[i][j]);

		fprintf(plik_out, "\n");
        }

    fclose(plik_out);
    }

    MPI_Finalize();
    return 0;
}
