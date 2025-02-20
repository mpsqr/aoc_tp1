#include <stdio.h>
#include <stdlib.h> // atoi, qsort
#include <stdint.h>

#define NB_METAS 31

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

static int cmp_uint64 (const void *a, const void *b) {
   const uint64_t va = *((uint64_t *) a);
   const uint64_t vb = *((uint64_t *) b);

   if (va < vb) return -1;
   if (va > vb) return 1;
   return 0;
}

int main (int argc, char *argv[]) {
   /* check command line arguments */
   if (argc != 4) {
      fprintf (stderr, "Usage: %s <size> <nb warmup repets> <nb measure repets>\n", argv[0]);
      return EXIT_FAILURE;
   }

   /* get command line arguments */
   const unsigned size = atoi (argv[1]); /* problem size */
   const unsigned repw = atoi (argv[2]); /* number of warmup repetitions */
   const unsigned repm = atoi (argv[3]); /* number of repetitions during measurement */

   uint64_t tdiff [NB_METAS];

   unsigned m;
   for (m=0; m<NB_METAS; m++) {
      printf ("Metarepetition %u/%d: running %u warmup instances and %u measure instances\n", m+1, NB_METAS,
              m == 0 ? repw : 1, repm);

      unsigned i;

      /* allocate arrays. TODO: adjust for each kernel */
      double (*a)[size] = malloc(size * size * sizeof(double));
      float *b = malloc(size * sizeof(float));
      float *c = malloc(size * sizeof(float));


      /* init arrays */
      srand(0);
      init_array (size, a);
      init_array_1d (size, b);

      /* warmup (repw repetitions in first meta, 1 repet in next metas) */
      if (m == 0) {
         for (i=0; i<repw; i++)
            kernel(size, a, b, c);
      } else {
         kernel(size, a, b, c);
      }

      /* measure repm repetitions */
      const uint64_t t1 = rdtsc();
      for (i=0; i<repm; i++) {
         kernel(size, a, b, c);
      }
      const uint64_t t2 = rdtsc();
      tdiff[m] = t2 - t1;

      /* free arrays. TODO: adjust for each kernel */
      free (a);
      free (b);
      free (c);
   }

   const unsigned nb_inner_iters = repm * (size * size * size); // TODO adjust for each kernel
   qsort (tdiff, NB_METAS, sizeof tdiff[0], cmp_uint64);

   // Minimum value: should be at least 2000 RDTSC-cycles
   const uint64_t min = tdiff[0];
   if (min < 2000) {
      fprintf (stderr, "Time for the fastest metarepet. is less than 2000 RDTSC-cycles.\n"
               "Rerun with more measure-repetitions\n");
      return EXIT_FAILURE;
   }
   printf ("MIN %lu RDTSC-cycles (%.2f per inner-iter)\n",
           min, (float) min / nb_inner_iters);

   // Median value
   const uint64_t med = tdiff[NB_METAS/2];
   printf ("MED %lu RDTSC-cycles (%.2f per inner-iter)\n",
           med, (float) med / nb_inner_iters);

   // Stability: (med-min)/min
   const float stab = (med - min) * 100.0f / min;
   if (stab >= 10)
      printf ("BAD STABILITY: %.2f %%\n", stab);
   else if (stab >= 5)
      printf ("AVERAGE STABILITY: %.2f %%\n", stab);
   else
      printf ("GOOD STABILITY: %.2f %%\n", stab);

   return EXIT_SUCCESS;
}
