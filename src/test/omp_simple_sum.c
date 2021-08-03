/*
 * MIT License Copyright(c) 2011-2020 The Maintainers of Nanvix
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

int __main2()
{
    int accumulate = 0;
    uint64_t time_start,time_final;

    time_start = clock_read();
    for(int i=0;i<(900000000/4)*1;i++)
        accumulate++;
    time_final = clock_read();

    uprintf("result = %d",accumulate);
    uprintf("the serial version runs with %d ticks",(int)(time_final-time_start) );
    return (0);
}
/*
int __main2()
{
    int accumulate = 0,thread_count = 2;
    uint64_t time_start,time_final;
    time_start = clock_read();

    #pragma omp parallel for num_threads(thread_count) reduction(+:accumulate)
    for(int i=0;i<(900000000/4)*1;i++)
        accumulate++;
    time_final = clock_read();

    uprintf("result = %d",accumulate);
    uprintf("the serial version runs with %d ticks",(int)(time_final-time_start) );
    return (0);
}
*/
/*
kthread_t thread_handles[THREAD_MAX];
struct nanvix_mutex protectSum;
int accumulate = 0;
int thread_count = 2;
int n = (900000000/2)*3;

int acumulator(void* rank)
{
   int my_sum=0;
   int my_n = n/thread_count;
   int my_rank = (int)(intptr_t) rank;
   int my_first_i = my_n*my_rank;
   int my_last_i = my_first_i + my_n;
   for(int i=my_first_i;i<my_last_i;i++)
   {
       my_sum++;
   }

   nanvix_mutex_lock(&protectSum);
   accumulate+=my_sum;
   nanvix_mutex_unlock(&protectSum);
   return accumulate;
}
int __main2()
{
   uint64_t time_start,time_final;
   int thread;
   nanvix_mutex_init(&protectSum,NULL);

   time_start = clock_read();
   for (thread = 0; thread < thread_count-1; thread++)  
      kthread_create(&thread_handles[thread],(void*)acumulator, (void*)thread+1);  
   
   acumulator(0);

   for (thread = 0; thread < thread_count-1; thread++) 
      kthread_join(thread_handles[thread], NULL); 
   accumulate+=n%thread_count;
   time_final = clock_read();

   uprintf("result = %d",accumulate);
   uprintf("the serial version runs with %d ticks",(int)(time_final-time_start) );
   return (0);
}
*/
