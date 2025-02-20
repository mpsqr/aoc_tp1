#include <stdio.h>
#include <stdlib.h> // atoi
#include <stdint.h>

extern uint64_t rdtsc ();

// TODO: adjust for each kernel
extern void kernel (unsigned n , double a[n][n] , const float b[n] , const float c[n]);

// TODO: adjust for each kernel
static void init_array (int n, double a[n][n]) {
   int i, j;

   for (i=0; i<n; i++)
      for (j=0; j<n; j++)
         a[i][j] = (double) rand() / RAND_MAX;
}

static void init_array_1d (int n, float b[n]) {
   int i;

   for (i=0; i<n; i++)
      b[i] = (float) rand() / RAND_MAX;
}

/*
// TODO: adjust for each kernel
static void print_array (int n, double a[n][n], const char *output_file_name) {
   int i, j;

   FILE *fp = fopen (output_file_name, "w");
   if (fp == NULL) {
      fprintf (stderr, "Cannot write to %s\n", output_file_name);
      return;
   }

   for (i=0; i<n; i++)
      for (j=0; j<n; j++)
         fprintf (fp, "%f\n", a[i][j]);

   fclose (fp);
}
*/

static void print_array_1d (int n, float a[n], const char *output_file_name) {
   int i;

   FILE *fp = fopen (output_file_name, "w");
   if (fp == NULL) {
      fprintf (stderr, "Cannot write to %s\n", output_file_name);
      return;
   }

   for (i=0; i<n; i++)
      fprintf (fp, "%f\n", a[i]);

   fclose (fp);
}

int main (int argc, char *argv[]) {
   /* check command line arguments */
   if (argc != 3) {
      fprintf (stderr, "Usage: %s <size> <output file name>\n", argv[0]);
      return EXIT_FAILURE;
   }

   /* get command line arguments */
   const unsigned size = atoi (argv[1]); /* problem size */
   const char *output_file_name = argv[2];

   /* allocate arrays. TODO: adjust for each kernel */
   double (*a)[size] = malloc(size * size * sizeof(double));
   float *b = malloc(size * sizeof(float));
   float *c = malloc(size * sizeof(float));

   /* init arrays */
   srand(0);
   init_array (size, a);
   init_array_1d (size, b);

   /* print output */
   kernel (size, a, b, c);
   print_array_1d (size, c, output_file_name);

   /* free arrays. TODO: adjust for each kernel */
   free (a);
   free (b);
   free (c);

   return EXIT_SUCCESS;
}
