/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <nanvix/ulib.h>
#include <nanvix/sys/thread.h>
#include <libgomp/omp.h>

void hello(void);
void atomic(void);
void critical(void);
void critical2(void);
void parallel_for(void);

int __main2(int argc, const char *argv[])
{
	((void) argc);
	((void) argv);

    hello();
    hello();

	return (0);
}

void hello(void)
{
	#pragma omp parallel num_threads(THREAD_MAX)
	uprintf("hello from thread %d of %d", omp_get_thread_num(),omp_get_num_threads());
}

void atomic(void)
{
    int atom_count = 0;

    #pragma omp parallel num_threads(THREAD_MAX) shared(atom_count)
    {
        #pragma omp atomic
        atom_count++;
    }
    uprintf("Sum value of atomic critical region is: %d", atom_count);
}

void critical(void)
{
    int crit_count = 0;
    #pragma omp parallel num_threads(THREAD_MAX) shared(crit_count) default(none)
    {
        #pragma omp critical
            crit_count++;
    }
    uprintf("Sum value of critical region is: %d", crit_count);
}
void critical2(void)
{
    int crit_count = 0;
    int crit_sub = THREAD_MAX;
    #pragma omp parallel num_threads(THREAD_MAX) shared(crit_count,crit_sub) default(none)
    {
        #pragma omp critical (name1)
            crit_count++;
        #pragma omp critical (name2)
            crit_sub--;

    }
    uprintf("Sum value of critical region is: %d final decremet is %d", crit_count, crit_sub);
}

void parallel_for(void)
{
    int pf_count = 0;
    int sum = 0;

    #pragma omp parallel num_threads(THREAD_MAX) firstprivate(pf_count) reduction(+:sum)
    {
        #pragma omp for schedule(static,2)
        for(int i=0;i<100;i++)
        {
            for (int j=0;j<20;j++)
            {
                #pragma omp atomic
                pf_count++;
            }
        }
        sum += pf_count;
        uprintf("value in thread %d: %d",omp_get_thread_num(), pf_count);
    }

       uprintf("final value: %d", sum);
}
