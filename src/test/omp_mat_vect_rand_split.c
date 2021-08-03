/* File:     
 *     omp_mat_vect_rand_split.c 
 *
 * Purpose:  
 *     Computes a parallel matrix-vector product.  Matrix
 *     is distributed by block rows.  Vectors are distributed by 
 *     blocks.  This version uses a random number generator to
 *     generate A and x.  There is some optimization.
 *
 * Compile:  
 *    gcc -g -Wall -fopenmp -o omp_mat_vect_rand_split 
 *          omp_mat_vect_rand_split.c 
 * Run:
 *    ./omp_mat_vect_rand_split <thread_count> <m> <n>
 *
 * Input:
 *     None unless compiled with DEBUG flag.
 *     With DEBUG flag, A, x
 *
 * Output:
 *     y: the product vector
 *     Elapsed time for the computation
 *
 * Notes:  
 *     1.  Storage for A, x, y is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both 
 *         m and n.  The program doesn't check for this.
 *     3.  We use a 1-dimensional array for A and compute subscripts
 *         using the formula A[i][j] = A[i*n + j]
 *     4.  Distribution of A, x, and y is logical:  all three are 
 *         globally shared.
 *     5.  DEBUG compile flag will prompt for input of A, x, and
 *         print y
 *
 * IPP:  Exercise 5.12
 */

#include <nanvix/ulib.h>
#include <libgomp/omp.h>

#define RAND_MAX 99

/* Serial functions */
void Get_args(int argc, char* argv[], int* thread_count_p, 
      int* m_p, int* n_p);
//void Gen_matrix(int A[], int m, int n);
void Gen_vector(int x[], int n);
void Print_matrix(char* title, int A[], int m, int n);
void Print_vector(char* title, int y[], int m);
void gambi_mult(int x[],int y[],int xDIM,int matDIM,int thread_count);

/* Parallel function */
void Omp_mat_vect(int A[], int x[], int y[],
      int m, int n, int thread_count);

/*------------------------------------------------------------------*/
int __main2() {
   int     thread_count=2;
   int     m=10000, n=4*1000;
   int* A;
   int* x;
   int* y;

   A = umalloc(m*n*sizeof(int));
   x = umalloc(n*sizeof(int));
   y = umalloc(m*sizeof(int));
   
   //Gen_matrix(A, m, n);
 /*Print_matrix("We generated", A, m, n); */
   Gen_vector(x, n);
 /*Print_vector("We generated", x, n); */

   //Omp_mat_vect(A, x, y, m, n, thread_count);
   gambi_mult(x, y, m, n, thread_count);


#  ifdef DEBUG
   Print_vector("The product is", y, m);
#  else
 /*Print_vector("The product is", y, m); */
#  endif

   ufree(A);
   ufree(x);
   ufree(y);

   return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function: Gen_matrix
 * Purpose:  Use the random number generator random to generate
 *    the entries in A
 * In args:  m, n
 * Out arg:  A
 */
void Gen_matrix(int A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = urand()%((int) RAND_MAX);
}  /* Gen_matrix */

/*------------------------------------------------------------------
 * Function: Gen_vector
 * Purpose:  Use the random number generator random to generate
 *    the entries in x
 * In arg:   n
 * Out arg:  A
 */
void Gen_vector(int x[], int n) {
   int i;
   for (i = 0; i < n; i++)
      x[i] = urand()%((int) RAND_MAX);
}  /* Gen_vector */

/*------------------------------------------------------------------
 * Function:  Omp_mat_vect
 * Purpose:   Multiply an mxn matrix by an nx1 column vector
 * In args:   A, x, m, n, thread_count
 * Out arg:   y
 */
void Omp_mat_vect(int A[], int x[], int y[],
      int m, int n, int thread_count) {
   int i, j;
   int start, finish, elapsed, temp;

   //GET_TIME(start);
   start = clock_read();
#  pragma omp parallel for num_threads(thread_count) \
      default(none) private(i, j, temp)  shared(A, x, y, m, n)
   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++) {
         temp = A[i*n+j]*x[j];
         y[i] += temp;
      }
   }

   //GET_TIME(finish);
   finish = clock_read();
   elapsed = finish - start;
   uprintf("Elapsed time = %d seconds\n", elapsed);

}  /* Omp_mat_vect */

void gambi_mult(int x[],int y[],int matDIM,int xDIM,int thread_count)
{

   int i, j;
   int start, finish, elapsed, temp;

   //GET_TIME(start);
   start = clock_read();
#  pragma omp parallel for num_threads(thread_count) default(none) private(i, j, temp)  shared( x, y, matDIM,xDIM) 
   for (i = 0; i < matDIM; i++) {
      y[i] = 0;
      for (j = 0; j < xDIM; j++) {
         temp = x[j]*x[j];
         y[i] += temp;
      }
   }

   //GET_TIME(finish);
   finish = clock_read();
   elapsed = finish - start;
   uprintf("using %d threads, Elapsed time = %d ticks\n",thread_count,elapsed);


}


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix( char* title, int A[], int m, int n) {
   int   i, j;

   uprintf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         uprintf("%d ", A[i*n + j]);
      uprintf("\n");
   }
}  /* Print_matrix */


/*------------------------------------------------------------------
 * Function:    Print_vector
 * Purpose:     Print a vector
 * In args:     title, y, m
 */
void Print_vector(char* title, int y[], int m) {
   int   i;

   uprintf("%s\n", title);
   for (i = 0; i < m; i++)
      uprintf("%d ", y[i]);
   uprintf("\n");
}  /* Print_vector */
