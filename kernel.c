#include <math.h> // pow , log
#include <stdio.h>


/* original */
void kernel (unsigned n , double *a, const float b[n] , const float c[n]) {
   unsigned i , j;
   for ( i =0; i < n ; i ++) {
      int ind = i * n;
      for ( j =0; j < n ; j ++) {
         a[ind + j] = log(pow(b[i] , c[j]));
      }
   }
}