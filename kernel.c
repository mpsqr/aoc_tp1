#include <math.h> // pow , log
#include <stdio.h>
#include <omp.h>


/* original */
void kernel (unsigned n , double *a, const float b[n] , const float c[n]) {
   unsigned i , j;

   #pragma omp parallel for private(j) shared(a, b, c, n)
   for ( i =0; i < n ; i ++) {
      int ind = i * n;
      for ( j =0; j < n ; j ++) {
         a[ind + j] = log(pow(b[i] , c[j]));
      }
   }
}