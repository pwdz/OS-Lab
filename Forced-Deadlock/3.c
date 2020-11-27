#include <stdio.h>
#include <omp.h>

#define THREAD_COUNT 4
int main(){
    omp_lock_t locka, lockb;

    omp_init_lock(&locka);
    omp_init_lock(&lockb);

    omp_set_num_threads(THREAD_COUNT);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        omp_set_lock(&locka);
        
        #pragma omp barrier
        omp_set_lock(&lockb);
        
        
        for(int i=0; i<=id;i++){
            printf("Thread num{%d}: %d\n",id, i);
        }

        omp_unset_lock(&locka);
        omp_unset_lock(&lockb);
        
    }

    omp_destroy_lock(&locka);
    omp_destroy_lock(&lockb);
    
    printf("Finished!\n");

    return 0;
}