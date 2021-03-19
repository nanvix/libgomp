/* Copyright (C) 2005, 2006 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License
   along with libgomp; see the file COPYING.LIB.  If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.  */

/* As a special exception, if you link this library with other files, some
   of which are compiled with GCC, to produce an executable, this library
   does not by itself cause the resulting executable to be covered by the
   GNU General Public License.  This exception does not however invalidate
   any other reasons why the executable file might be covered by the GNU
   General Public License.  */

/* This is the default POSIX 1003.1b implementation of a semaphore
   synchronization mechanism for libgomp.  This type is private to
   the library.

   This is a bit heavy weight for what we need, in that we're not
   interested in sem_wait as a cancelation point, but it's not too
   bad for a default.  */

#ifndef GOMP_SEM_H
#define GOMP_SEM_H 1

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility push(default)
#endif

#include "workaround.h"
//#include <semaphore.h>
#include <nanvix/sys/semaphore.h>

#ifdef HAVE_ATTRIBUTE_VISIBILITY
# pragma GCC visibility pop
#endif

#ifdef HAVE_BROKEN_POSIX_SEMAPHORES
//#include <pthread.h>
#include <nanvix/sys/mutex.h>
#include <nanvix/sys/condvar.h>

/////////// dumb declarations //////////////
typedef struct nanvix_mutex pthread_mutex_t;
//////////////////////////////////////

struct gomp_sem
{
  pthread_mutex_t	mutex;
  struct nanvix_cond_var cond;
  int			value;
};

typedef struct gomp_sem gomp_sem_t;

extern void gomp_sem_init (gomp_sem_t *sem, int value);

extern void gomp_sem_wait (gomp_sem_t *sem);

extern void gomp_sem_post (gomp_sem_t *sem);

extern void gomp_sem_destroy (gomp_sem_t *sem);







#else /* HAVE_BROKEN_POSIX_SEMAPHORES  */



typedef struct nanvix_semaphore gomp_sem_t;

static inline void gomp_sem_init (gomp_sem_t *sem, int value)
{
  nanvix_semaphore_init (sem,  value);
}

extern void gomp_sem_wait (gomp_sem_t *sem);

static inline void gomp_sem_post (gomp_sem_t *sem)
{
    nanvix_semaphore_up (sem);
}

static inline void gomp_sem_destroy (gomp_sem_t  *sem)
{
  nanvix_semaphore_destroy (sem);
}
#endif /* doesn't HAVE_BROKEN_POSIX_SEMAPHORES  */
#endif /* GOMP_SEM_H  */
