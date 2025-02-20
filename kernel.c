#include <math.h> // pow , log
#include <stdio.h>


/* original */
void kernel (unsigned n , double *a, const float b[n] , const float c[n]) {
   unsigned i , j;
   for ( j =0; j < n ; j ++) {
      for ( i =0; i < n ; i ++) {
         a[i*n + j] = log(pow(b[i] , c[j]));
      }
   }
}