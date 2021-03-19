/* Copyright (C) 2005, 2006 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU OpenMP Library (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
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

/* This file defines the OpenMP internal control variables, and arranges
   for them to be initialized from environment variables at startup.  */

#include <libgomp/libgomp.h>
#include <libgomp/libgomp_f.h>
#include <nanvix/ulib.h>

#include <libgomp/limits.h>
#include <posix/errno.h>


unsigned long gomp_nthreads_var = 1;
bool gomp_dyn_var = false;
bool gomp_nest_var = false;
enum gomp_schedule_type gomp_run_sched_var = GFS_DYNAMIC;
unsigned long gomp_run_sched_chunk = 1;

static void __attribute__((constructor))
initialize_env (void)
{
    gomp_init_num_threads ();
}

/* The public OpenMP API routines that access these variables.  */

void
omp_set_num_threads (int n)
{
  gomp_nthreads_var = (n > 0 ? n : 1);
}

void
omp_set_dynamic (int val)
{
  gomp_dyn_var = val;
}

int
omp_get_dynamic (void)
{
  return gomp_dyn_var;
}

void
omp_set_nested (int val)
{
  gomp_nest_var = val;
}

int
omp_get_nested (void)
{
  return gomp_nest_var;
}

ialias (omp_set_dynamic)
ialias (omp_set_nested)
ialias (omp_set_num_threads)
ialias (omp_get_dynamic)
ialias (omp_get_nested)
