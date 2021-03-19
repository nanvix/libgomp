/* This header is used during the build process to find the size and 
   alignment of the public OpenMP locks, so that we can export data
   structures without polluting the namespace.

   In this default POSIX implementation, we map the two locks to the
   same PTHREADS primitive.  */

#include <nanvix/sys/mutex.h>

typedef pthread_mutex_t omp_lock_25_t;
typedef struct { pthread_mutex_t lock; int count; } omp_nest_lock_25_t;
typedef struct nanvix_mutex omp_lock_t;
typedef struct {struct nanvix_mutex lock; int count; } omp_nest_lock_t;
