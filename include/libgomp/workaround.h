#include <nanvix/ulib.h>
#include "mutex.h"
#include <nanvix/sys/mutex.h>

#ifndef WORKAROUND_H
#define WORKAROUND_H 1
typedef struct nanvix_mutex nanvix_mutex_t;
typedef unsigned int pthread_key_t;

//# define __SIZEOF_PTHREAD_MUTEX_T 24
# define __SIZEOF_PTHREAD_ATTR_T 36
# define __SIZEOF_PTHREAD_RWLOCK_T 32
# define __SIZEOF_PTHREAD_BARRIER_T 20
#define __SIZEOF_PTHREAD_MUTEXATTR_T 4
#  define __SIZEOF_PTHREAD_MUTEX_T 40
#define stderr 0
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define __WORDSIZE  32

#if __WORDSIZE == 64
# define __SIZEOF_SEM_T	32
#else
# define __SIZEOF_SEM_T	16
#endif
#define MEMMODEL_SYNC (1<<15)


//struct tls_data
//{
//    kthread_t key;
//    gomp_thread data;
//}
//tls_data* tls[THREAD_MAX];


/* Memory model types for the __atomic* builtins.
   This must match the order in libstdc++-v3/include/bits/atomic_base.h.  */
enum memmodel
{
  MEMMODEL_RELAXED = 0,
  MEMMODEL_CONSUME = 1,
  MEMMODEL_ACQUIRE = 2,
  MEMMODEL_RELEASE = 3,
  MEMMODEL_ACQ_REL = 4,
  MEMMODEL_SEQ_CST = 5,
  MEMMODEL_LAST = 6,
  MEMMODEL_SYNC_ACQUIRE = MEMMODEL_ACQUIRE | MEMMODEL_SYNC,
  MEMMODEL_SYNC_RELEASE = MEMMODEL_RELEASE | MEMMODEL_SYNC,
  MEMMODEL_SYNC_SEQ_CST = MEMMODEL_SEQ_CST | MEMMODEL_SYNC,
  /* Say that all the higher bits are valid target extensions.  */
//  MEMMODEL_MAX = INTTYPE_MAXIMUM (int)
  MEMMODEL_MAX = 0x80000U
};


////pthread types////
typedef struct nanvix_mutex pthread_mutex_t;
typedef union
{
  char __size[__SIZEOF_SEM_T];
  long int __align;
} sem_t;



/////end pthread types


//extern int pthread_attr_getstacksize (pthread_attr_t *__attr,int * stacksize);
//extern int pthread_attr_setstacksize (pthread_attr_t *__attr,size_t  stacksize);
//

extern void initialize_team (void);
extern struct gomp_thread* pthread_getspecific (pthread_key_t key);

extern int pthread_setspecific (pthread_key_t key, struct gomp_thread *__pointer);

extern int pthread_key_create (pthread_key_t *__key,
			       void (*__destr_function) (void *));


/* Destroy KEY.  */
extern int pthread_key_delete (pthread_key_t __key);// __THROW;

extern int pthread_detach (kthread_t __th);


extern void fputs(char* str, void*);
extern void fputc(char str, void*);
//extern int nanvix_mutex_trylock (nanvix_mutex_t *__mutex);
//extern int nanvix_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind);

//critical lock global
static struct nanvix_mutex default_lock;
static struct nanvix_mutex atomic_lock;
void initialize_critical (void);

#endif
