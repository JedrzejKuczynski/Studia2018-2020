#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
   FILE* plik;
   int i, j;

   int numbers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   plik = fopen("liczby.txt", "w");

   if(plik == NULL) {
      printf("Blad otwarcia pliku \"liczby.txt\"\n");
      exit(0);
   }

   for(i = 0; i < 2000; i++) {
      for(j = 0; j < 2000; j++)
         fprintf(plik, "%f ", (float)numbers[i % 10]);

      fprintf(plik, "\n");
   }

   fclose(plik);
   return 0;
}
