#include <nanvix/sys/mutex.h>
#include <nanvix/sys/thread.h>
#include <libgomp/libgomp.h>

/* Return data of a Specific thread called by kthread_self() */
struct gomp_thread* pthread_getspecific (pthread_key_t key)
{
	return tls_omp[kthread_self()].data;
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
   for(int i=0;i<100;i++)
   {
		tls_omp[i].key = (kthread_t) i;
		tls_omp[i].data = umalloc(sizeof(struct gomp_thread*));
   }
   return 0;
}
extern int pthread_detach (kthread_t __th)
{
	(void) __th;
	uprintf("%s \n",__func__);
	return 0;
}

extern void fputs(char* str, void* var)
{
	(void) str;
	(void) var;
	uprintf("%s \n",__func__);
}
extern void fputc(char str, void* c)
{
	(void) str;
	(void) c;
	uprintf("%s \n",__func__);
}

void initialize_critical (void)
{
  gomp_mutex_init (&default_lock);
  gomp_mutex_init (&atomic_lock);
}
