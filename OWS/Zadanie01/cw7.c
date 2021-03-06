#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 4000
#define PP 2
#define P 4


// const int n = 2000, PP = 2, P = 4;  // wielkosc mnozonych macierzy, pierwiastek z liczby procesow, liczba procesow

float a[n][n], b[n][n], c[n][n];
float aa[n / PP][n / PP], bb[n / PP][n / PP], cc[n / PP][n / PP];
float tmp_submatrix[n / PP][n / PP];

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

    // Wczytanie danych przez proces rank = 0 oraz wysylanie odpowiednich podmacierzy reszcie procesow
    if(my_rank == 0) {
        plik = fopen("liczby.txt", "r");
        if(plik == NULL) {
            printf("Blad otwarcia pliku \"liczby.txt\"\n");
            koniec = 1;
            MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Finalize();
            exit(0);
        }else {
            koniec = 0;
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
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++) {
                fscanf(plik,"%f", &a[i][j]);
            }

        fclose(plik);

        // Rozsylanie odpowiednich podmacierzy A reszcie procesow

        for(int i = 0; i < PP; i++)
            for(int j = 0; j < PP; j++) {
                int process_rank = i * PP + j;

                if(process_rank != 0) {
                    for(int ii = i * (n / PP); ii < (i + 1) * (n / PP); ii++)
                        for(int jj = j * (n / PP); jj < (j + 1) * (n / PP); jj++)
                            tmp_submatrix[ii - (i * (n / PP))][jj - (j * (n / PP))] = a[ii][jj];

                MPI_Isend(tmp_submatrix, n*n / P, MPI_FLOAT, process_rank, tag, MPI_COMM_WORLD, reqSend);
            }
        }

        // Tworzenie wlasne podmacierzy, tj dla procesu 0

        for(int ii = my_rank * (n / PP); ii < (my_rank + 1) * (n / PP); ii++)
            for(int jj = my_rank * (n / PP); jj < (my_rank + 1) * (n / PP); jj++)
                aa[ii - (my_rank * (n / PP))][jj - (my_rank * (n / PP))] = a[ii][jj];

        plik = fopen("liczby.txt", "r");

        // Analogicznie dla macierzy B

        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++) {
                fscanf(plik,"%f", &b[i][j]);
        }

        fclose(plik);

        for(int i = 0; i < PP; i++)
            for(int j = 0; j < PP; j++) {
                int process_rank = i * PP + j;

                if(process_rank != 0) {
                    for(int ii = i * (n / PP); ii < (i + 1) * (n / PP); ii++)
                        for(int jj = j * (n / PP); jj < (j + 1) * (n / PP); jj++)
                            tmp_submatrix[ii - (i * (n / PP))][jj - (j * (n / PP))] = b[ii][jj];

                MPI_Isend(tmp_submatrix, n*n / P, MPI_FLOAT, process_rank, tag, MPI_COMM_WORLD, &reqSend[1]);
            }
        }

        for(int ii = my_rank * (n / PP); ii < (my_rank + 1) * (n / PP); ii++)
            for(int jj = my_rank * (n / PP); jj < (my_rank + 1) * (n / PP); jj++)
                bb[ii - (my_rank * (n / PP))][jj - (my_rank * (n / PP))] = b[ii][jj];

    }else {  // Odbieranie podmacierzy przez reszte procesow

    	MPI_Irecv(aa, n*n / P, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, reqRecv);
        MPI_Wait(reqRecv, statRecv);

    	MPI_Irecv(bb, n*n / P, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &reqRecv[1]);
        MPI_Wait(&reqRecv[1], &statRecv[1]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Zerowanie podmacierzy wynikowej

    for(int i = 0; i < n / PP; i++)
        for(int j = 0; j < n / PP; j++)
            cc[i][j] = 0;

    MPI_Barrier(MPI_COMM_WORLD);

    // ---- MNOZENIE CANNONA ---- //

    // Okreslanie wlasnej pozycji i sasiadow w kracie toroidalnej

    row = my_rank / PP;
    col = my_rank % PP;

    int right = (row * PP) + (col + 1) % PP;
    int bottom = ((row + 1) % PP) * PP + col;
    int top = ((row + PP - 1) % PP) * PP + col;
    int left = (row * PP) + (col - 1 + PP) % PP;

    if(my_rank == 0)
        startwtime2 = MPI_Wtime();  //czas w sekundach

    // Przesuwanie rzedu "i" macierzy A "i" razy (zgodnie ze struktura kraty toroidalnej, cyklicznie)

    for(int i = 0; i < PP; i++) {
        if(row > i) {
            MPI_Isend(aa, n*n / P, MPI_FLOAT, left, tag, MPI_COMM_WORLD, reqSend);
            MPI_Irecv(aa, n*n / P, MPI_FLOAT, right, tag, MPI_COMM_WORLD, reqRecv);
            MPI_Wait(reqRecv, statRecv);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Przesuwanie kolumny "i" macierzy B "i" razy (zgodnie ze struktura kraty toroidalnej, cyklicznie)

    for(int i = 0; i < PP; i++) {
        if(col > i) {
            MPI_Isend(bb, n*n / P, MPI_FLOAT, top, tag, MPI_COMM_WORLD, &reqSend[1]);
            MPI_Irecv(bb, n*n / P, MPI_FLOAT, bottom, tag, MPI_COMM_WORLD, &reqRecv[1]);
            MPI_Wait(&reqRecv[1], &statRecv[1]);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Dzieki temu poczatkowemu przesunieciu odpowiednie fragmenty macierzy A[i, k]
    // oraz fragmenty macierzy B[k, j] znajduja sie w procesie P[i, j]
    // gdzie "i", "j" i "k" to wspolrzedne w kracie toroidalnej


    for(int pr = 0; pr < PP; pr++) { // Iteracja przetwarzania
        for(int i = 0; i < n / PP; i++)
            for(int k = 0; k < n / PP; k++)
                for(int j = 0; j < n / PP; j++)
                    cc[i][j] += aa[i][k] * bb[k][j];  // Obliczanie wyniku czesciowego w ramach swoich podtablic

        // Rozsylanie podmacierzy A lewemu sasiadowi a podmiacierzy B gornemu (zgodnie ze struktura kraty toroidalnej, cyklicznie)
        // A odbieranie od prawego i dolnego
        // W celu obliczenia dalszych fragmentów macierzy wynikowej

        if(pr < PP - 1) {
            MPI_Isend(aa, n*n / P, MPI_FLOAT, left, tag, MPI_COMM_WORLD, reqSend);
            MPI_Irecv(aa, n*n / P, MPI_FLOAT, right, tag, MPI_COMM_WORLD, reqRecv);
            MPI_Wait(reqRecv, statRecv);
            MPI_Barrier(MPI_COMM_WORLD);

            MPI_Isend(bb, n*n / P, MPI_FLOAT, top, tag, MPI_COMM_WORLD, &reqSend[1]);
            MPI_Irecv(bb, n*n / P, MPI_FLOAT, bottom, tag, MPI_COMM_WORLD, &reqRecv[1]);
            MPI_Wait(&reqRecv[1], &statRecv[1]);
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }

    if(my_rank == 0) {
        endwtime = MPI_Wtime();
        printf("Calkowity czas przetwarzania wynosi %f sekund\n", endwtime - startwtime1);
        printf("Calkowity czas obliczen wynosi %f sekund\n", endwtime - startwtime2);
    } else
        MPI_Isend(cc, n*n / P, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, reqSend);  // Rosylanie fragmentow macierzy wynikowej do procesu 0

    if(my_rank == 0) {

        // Zbieranie macierzy wynikowej

        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                c[i][j] = 0;

        int start_row = (my_rank / PP) * (n / PP);
        int start_col = (my_rank % PP) * (n / PP);

        for(int i = 0; i < n / PP; i++)
            for(int j = 0; j < n / PP; j++)
                c[i + start_row][j + start_col] = cc[i][j];

        for(int r = 1; r < P; r++) {
            MPI_Recv(cc, n*n / P, MPI_FLOAT, r, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            start_row = (r / PP) * (n / PP);
            start_col = (r % PP) * (n / PP);

            for(int i = 0; i < n / PP; i++)
                for(int j = 0; j < n / PP; j++)
                    c[i + start_row][j + start_col] = cc[i][j];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(my_rank == 0) {

        /*
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++)
                printf("%lf\t", c[i][j]);
            printf("\n");
        }*/

        plik_out = fopen("wynik.txt", "w");

        if(plik_out == NULL) {
            printf("Blad otwarcia pliku \"wynik.txt\"\n");
            exit(0);
        }

        for(int i = 0; i < n; i++) {
		  for(int j = 0; j < n; j++)
            fprintf(plik_out, "%f ", c[i][j]);

		fprintf(plik_out, "\n");
        }

    fclose(plik_out);
    }

    MPI_Finalize();
    return 0;
}
