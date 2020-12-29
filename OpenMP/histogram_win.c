#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

// #define THREAD_COUNT 4
int THREAD_COUNT;
#define MAX_NUM 10

void fill_array(long long n, long long **arr){
    time_t t;
    srand((unsigned) time(&t));
    for(long long i=0; i<n ; i++){
        for(long long j=0 ; j<n; j++){
            arr[i][j] = rand() % MAX_NUM;
        }
    }
}
void parallel_rows(long long n, long long **arr, long long *histogram){
    // long long temp[THREAD_COUNT][MAX_NUM] = {0};
    // #pragma omp parallel
    // {
    //     #pragma omp for
    //     for(long long i=0;  i<n;  i++){
    //         for (long long j = 0; j < n; j++)
    //         {
    //             #pragma omp critical
    //             histogram[arr[i][j]]+=1;
    //         }
            
    //     }
    // }


    #pragma omp parallel
    {
        long long* histogram_private = (long long *)malloc(sizeof(long long)*MAX_NUM);
        for(long long i=0; i<MAX_NUM; i++) 
            histogram_private[i] = 0;


        // printf("[#]i=%d; ;i+=%d\n",omp_get_thread_num(),THREAD_COUNT);
        long long tid = omp_get_thread_num();
        long long min = (n/omp_get_num_threads()) * tid;
        long long max = (n/omp_get_num_threads()) * (tid+1);
        // #pragma omp for nowait
        for(long long i=min; i<max; i++) {
           for(long long j=0; j<n; j++) {    
            //    printf("tid:%lld , %lld\n",tid,arr[i][j]);
               #pragma omp critical
               histogram[arr[i][j]]++;
            //    histogram_private[arr[i][j]]++;
           }
        }      
        // #pragma omp critical 
        // {

        //     // printf("[##]i=%lld; ;i+=%lld\n",omp_get_thread_num(),THREAD_COUNT);
        //     for(long long i=0; i<MAX_NUM; i++) 
        //         histogram[i] += histogram_private[i];
        // }
    }


    // #pragma omp parallel 
    // {
    //     printf("i=%lld; ;i+=%lld\n",omp_get_thread_num(),THREAD_COUNT);
    //     long long tid = omp_get_thread_num();
    //     long long private_histogram[MAX_NUM];
    //     for(long long i=0; i<MAX_NUM; i++)
    //         private_histogram[i] = 0;
        


    //     #pragma omp for nowait
    //     for(long long i=tid; i < n; i+=THREAD_COUNT ){
    //         for(long long j=0; j<n; j++){
    //             private_histogram[arr[i][j]] ++;
    //             // #pragma omp critical
    //         //    {
    //                 // histogram[arr[i][j]]++;
    //                 // printf("loooool!");
    //             // }
    //         }

    //     }
    //     #pragma omp critical
    //     {
    //         for(long long i=0; i<MAX_NUM; i++)
    //             histogram[i] += private_histogram[i];
    //     }
    // }

    // for(long long i=0; i<MAX_NUM; i++)
    //     for(long long j=0; j<THREAD_COUNT; j++)
    //         histogram[i] += temp[j][i];
    // printf("!!!!!!!!");
}
void parallel_columns(long long n, long long** arr, long long *histogram){
    // #pragma omp parallel
    // {
    //     #pragma omp for
    //     for(long long i=0;  i<n;  i++){
    //         for (long long j = 0; j < n; j++)
    //         {
    //             #pragma omp critical
    //             histogram[arr[j][i]]+=1;
    //         }
            
    //     }
    // }

    // int private_hist[omp_get_num_threads()][MAX_NUM];
    long long **private_hist = (long long**)malloc(n* sizeof(long long*));
    for (long long i = 0; i < n; i++) {
        private_hist[i] = (long long*)malloc(n * sizeof(long long));
    }


    for(int i=0; i<omp_get_num_threads(); i++){
        for(int j=0; j<MAX_NUM; j++)
            private_hist[i][j] = 0;
    }


    #pragma omp parallel
    {
        // long long* histogram_private = (long long *)malloc(sizeof(long long)*MAX_NUM);
        // for(long long i=0; i<MAX_NUM; i++) 
            // histogram_private[i] = 0;


        // printf("[#]i=%d; ;i+=%d\n",omp_get_thread_num(),THREAD_COUNT);
        long long tid = omp_get_thread_num();
        long long min = (n/omp_get_num_threads()) * tid;
        long long max = (n/omp_get_num_threads()) * (tid+1);
        // #pragma omp for nowait
        for(long long i=min; i<max; i++) {
           for(long long j=0; j<n; j++) {    
            //    printf("tid:%lld , %lld\n",tid,arr[i][j]);
               #pragma omp critical
               private_hist[tid][arr[i][j]]++;
            //    histogram_private[arr[i][j]]++;
           }
        }      
    }
    #pragma omp critical
    {
        puts("khodaya base dg");
        for(int i=0; i<omp_get_num_threads(); i++) {
            for(int j=0; j<MAX_NUM; j++)
                histogram[j] += private_hist[i][j];
        }
    }


    // #pragma omp for
    // for (long long i=0 ; i<n ; i++ ) {
    //     for (int j=0; j<n; j++){
    //         private_hist[omp_get_thread_num()][arr[i][j]] ++;
    //     }
    // }
}

void parallel_blocks(long long n, long long** arr, long long *histogram){

    #pragma omp parallel
    {
    }

}
long long main(){
    long long n;
    scanf("%lld,%d", &n, &THREAD_COUNT);
    puts("|::::::");
    long long **img = (long long**)malloc(n* sizeof(long long*));
    for (long long i = 0; i < n; i++) {
        img[i] = (long long*)malloc(n * sizeof(long long));
    }

    // long long img[n][n];
    
    fill_array(n, img);

    // long long histogram[MAX_NUM];
    long long* histogram = (long long*)malloc(sizeof(long long)*MAX_NUM);
    for(long long i=0; i<n; i++)
        histogram[i] = 0;

    struct timeval start, end; 
    gettimeofday(&start, NULL);
 
    omp_set_num_threads(THREAD_COUNT);
    // parallel_rows(n, img, histogram);
    parallel_columns(n, img, histogram);
    // parallel_blocks(n, img, histogram);

    // for(long long i=0; i<n; i++){
    //     for(long long j=0; j<n; j++){
    //         printf("%.3d ",img[i][j]);
    //     }
    //     printf("\n");
    // }

    for(long long i=0; i<MAX_NUM; i++){
        if(histogram[i]!=0)
        printf("%lld:%lld\n",i,histogram[i]);
    }
 
    gettimeofday(&end, NULL);
 
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
    printf("Time elpased %ld microseconds\n", micros);
  

    return 0;
}