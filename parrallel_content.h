//
// Created by zribktad  on 8. 10. 2022.
//

#ifndef SEM_PARRALLEL_CONTENT_H
#define SEM_PARRALLEL_CONTENT_H

#include <algorithm>
#include <thread>
#include <functional>
#include <vector>

#define PARALLEL_FOR_BEGIN_LOOP(nb_elements) parallel_for(nb_elements, [&](int start, int end, unsigned thread_num){ for(int i = start; i < end; ++i)
#define PARALLEL_TASKS_BEGIN(nb_elements) parallel_for_block(nb_elements, [&](unsigned thread_num){
#define PARALLEL_FOR_BEGIN(nb_elements) parallel_for(nb_elements, [&](int start, int end, unsigned thread_num){
#define PARALLEL_END()})
#define NUM_OF_TRREADS_NOW std::thread::hardware_concurrency()+1





static
void parallel_for_block(unsigned nb_elements,
                  std::function<void (unsigned thread_num)> functor,
                  bool use_threads = true)
{
    std::vector< std::thread > my_threads(nb_elements);
    if( use_threads )
    {
        for(unsigned i = 0; i < nb_elements; ++i)
        {
            my_threads[i] = std::thread(functor,i);
        }
    }
    else
    {
        for(unsigned i = 0; i < nb_elements; ++i){
            functor( i );
        }
    }

    if( use_threads )
        std::for_each(my_threads.begin(), my_threads.end(), std::mem_fn(&std::thread::join));
}

static
void parallel_for(unsigned nb_elements,
                  std::function<void (int start, int end, unsigned thread_num)> functor,
                  bool use_threads = true)
{
    unsigned nb_threads_hint = std::thread::hardware_concurrency();
    unsigned nb_threads = nb_threads_hint == 0 ? 8 : (nb_threads_hint);
    unsigned batch_size = nb_elements / nb_threads;
    unsigned batch_remainder = nb_elements % nb_threads;
    if(batch_size>0) {
        std::vector<std::thread> my_threads(nb_threads);
        if (use_threads) {
            for (unsigned i = 0; i < nb_threads; ++i) {
                int start = i * batch_size;
                my_threads[i] = std::thread(functor, start, start + batch_size, i);
            }
        } else {
            for (unsigned i = 0; i < nb_threads; ++i) {
                int start = i * batch_size;
                functor(start, start + batch_size, i);
            }
        }


    if( use_threads && batch_size > 0 )
        std::for_each(my_threads.begin(), my_threads.end(), std::mem_fn(&std::thread::join));
    }

    int start = nb_threads * batch_size;
    functor( start, start+batch_remainder,nb_threads);
}



#endif //SEM_PARRALLEL_CONTENT_H
