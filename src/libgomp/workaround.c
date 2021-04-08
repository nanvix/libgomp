#include <nanvix/sys/mutex.h>
#include <nanvix/sys/thread.h>
#include <libgomp/libgomp.h>

/* Return data of a Specific thread called by kthread_self() */
struct gomp_thread* pthread_getspecific (pthread_key_t key)
{
	return tls_omp[key].data;
}

/* Set a gomp thread pointer on team to use Thread Local Storage */
int pthread_setspecific (pthread_key_t  key, struct gomp_thread *__pointer)
{
    tls_omp[key].data = __pointer;
    return (0);
}

/* create desired thread key values on the team*/
int  pthread_key_create (pthread_key_t * key,void (*destructor)(void*))
{
   for(int i=0;i<MAX_TLS_THREADS;i++)
   {
		tls_omp[i].key = (kthread_t) i;
		tls_omp[i].data = NULL;
   }
   return 0;
}
int  pthread_key_delete (pthread_key_t  __key)
{
    for(int i=0;i<MAX_TLS_THREADS;i++)
    {
        tls_omp[i].key = 0;
//		ufree(tls_omp[i].data);
    }
    return 0;
}
