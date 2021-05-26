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

kthread_t tid[THREAD_MAX];



int __main2(){

    int n, i;
    int thread_count;
    int factor;
    int sum;
    int difTime=0;
    uint64_t time_start,time_final;
    thread_count = 3;
    n = 4*100000000;
    sum = 0;
    time_start = clock_read();
#pragma omp parallel for default(none) num_threads(thread_count) shared(n) private(factor) reduction(+:sum) 
    for (i = 0; i < n; i++)
    {
        factor = (i % 2 == 0) ? 1 : -1;
        sum += 1000000*factor/(2*i+1);
    }
    time_final = clock_read();
    difTime+=(time_final-time_start);
    //}
    sum = 4*sum;

    uprintf("With n = %d terms\n", n);
    uprintf("   Our estimate of pi = %d\n", sum);
    uprintf("time of execution: %d",(int)(difTime));
    return (0);

}

//int thread_count;
//int n;
//int sum;
//struct nanvix_mutex protectSum;
//
//void* Local_pi(void * rank)
//{
//   int my_rank = (int) rank;
//   int factor;
//   int i;
//   int my_n = n/thread_count;
//   int my_first_i = my_n*my_rank;
//   int my_last_i = my_first_i + my_n;
//   int my_sum=0;
//
//   factor = (my_first_i % 2 == 0) ? 1 : -1;
//
//   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
//        my_sum += 1000000*factor/(2*i+1);  
//   }
//
//   nanvix_mutex_lock(&protectSum);
//       sum += my_sum;
//   nanvix_mutex_unlock(&protectSum);
//
//   return NULL;
//}
//
//int __main2() {
//
//    int time_start,time_final;
//    n = 2*100000000;
//    thread_count = 2;
//    sum = 0;
//    nanvix_mutex_init(&protectSum,NULL);
//
//    int soma_escalabilidade = 0;
//    int       thread;  /* Use int in case of a 64-bit system */
//    kthread_t* thread_handles;
//    thread_handles = (kthread_t*) umalloc (thread_count*sizeof(kthread_t)); 
//
//    time_start = clock_read();
//    for (thread = 0; thread < thread_count-1; thread++)  
//       kthread_create(&thread_handles[thread],Local_pi, (void*)thread+1);  
//    
//    Local_pi((void*)0);
//
//    for (thread = 0; thread < thread_count-1; thread++) 
//       kthread_join(thread_handles[thread], NULL); 
//    time_final = clock_read();
//    soma_escalabilidade+=(time_final-time_start);
//
//    nanvix_mutex_destroy(&protectSum);
//   sum = 4*sum;
//   uprintf("With n = %d terms,\n", n);
//   uprintf("   Our estimate of pi = %d\n", sum);
//   uprintf("time of execution: %d",(int)(soma_escalabilidade));
//   
//   ufree(thread_handles);
//   return (0);
//}

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
    int sum_pf = 0;

    #pragma omp parallel num_threads(THREAD_MAX) firstprivate(pf_count) reduction(+:sum_pf)
    {
        #pragma omp for schedule(guided,2)
        for(int i=0;i<100;i++)
        {
            for (int j=0;j<20;j++)
            {
                #pragma omp atomic
                pf_count++;
            }
        }
        sum_pf += pf_count;
        uprintf("value in thread %d: %d",omp_get_thread_num(), pf_count);
    }

       uprintf("final value: %d", sum_pf);
}
