#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>


#define THREAD_COUNT 16 
#define MAX_NUM 256

void fill_array(int n, int** arr){
    time_t t;
    srand((unsigned) time(&t));
    for(int i=0; i<n ; i++){
        for(int j=0 ; j<n; j++){
            arr[i][j] = rand() % MAX_NUM;
        }
    }
}
void parallel_rows(int n, int **arr, int *histogram){
    // int temp[THREAD_COUNT][MAX_NUM] = {0};
    //#pragma omp parallel
    //{
        //#pragma omp for
        //for(int i=0;  i<n;  i++){
            //for (int j = 0; j < n; j++)
            //{
                //#pragma omp critical
                //histogram[arr[i][j]]+=1;
                
            //}
            
        //}
    //}

   

	
    #pragma omp parallel
    {
        long long* histogram_private = (long long *)malloc(sizeof(long long)*MAX_NUM);
        for(int i=0; i<MAX_NUM; i++) 
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
    //     int* histogram_private = (int *)malloc(sizeof(int)*MAX_NUM);
    //     for(int i=0; i<MAX_NUM; i++) 
    //         histogram_private[i] = 0;


    //     printf("[#]i=%d; ;i+=%d\n",omp_get_thread_num(),THREAD_COUNT);
    //     int tid = omp_get_thread_num();
        
    //     // #pragma omp for nowait
    //     for(int i=tid; i<n; i+=THREAD_COUNT) {
    //        for(int j=0; j<n; j++) {    
    //            printf("tid:%d , %d\n",tid,arr[i][j]);
    //            histogram_private[arr[i][j]]++;
    //        }
    //     }      
    //     #pragma omp critical 
    //     {

    //         printf("[##]i=%d; ;i+=%d\n",omp_get_thread_num(),THREAD_COUNT);
    //         for(int i=0; i<MAX_NUM; i++) 
    //             histogram[i] += histogram_private[i];
    //     }
    // }


    // #pragma omp parallel 
    // {
    //     printf("i=%d; ;i+=%d\n",omp_get_thread_num(),THREAD_COUNT);
    //     int tid = omp_get_thread_num();
    //     int private_histogram[MAX_NUM];
    //     for(int i=0; i<MAX_NUM; i++)
    //         private_histogram[i] = 0;
        


    //     #pragma omp for nowait
    //     for(int i=tid; i < n; i+=THREAD_COUNT ){
    //         for(int j=0; j<n; j++){
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
    //         for(int i=0; i<MAX_NUM; i++)
    //             histogram[i] += private_histogram[i];
    //     }
    // }

    // for(int i=0; i<MAX_NUM; i++)
    //     for(int j=0; j<THREAD_COUNT; j++)
    //         histogram[i] += temp[j][i];
    // printf("!!!!!!!!");
}
void parallel_columns(int n, int** arr, int *histogram){
    //#pragma omp parallel
    //{
        //#pragma omp for
        //for(int i=0;  i<n;  i++){
            //for (int j = 0; j < n; j++)
          //  {
        //        #pragma omp critical
      //          histogram[arr[j][i]]+=1;
    //        }
            
  //      }
//    }

	
    #pragma omp parallel
    {
        long long* histogram_private = (long long *)malloc(sizeof(long long)*MAX_NUM);
        for(int i=0; i<MAX_NUM; i++) 
            histogram_private[i] = 0;


        // printf("[#]i=%d; ;i+=%d\n",omp_get_thread_num(),THREAD_COUNT);
        long long tid = omp_get_thread_num();
        long long min = (n/omp_get_num_threads()) * tid;
        long long max = (n/omp_get_num_threads()) * (tid+1);
        // #pragma omp for nowait
        for(long long i=0; i<n; i++) {
           for(long long j=min; j<max; j++) {    
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




}

void parallel_blocks(int n, int** arr, int *histogram){
    int half = THREAD_COUNT/2;
    int coe = n / half;
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int minI,maxI,minJ,maxJ;
        if(sqrt(THREAD_COUNT) == (double)(int)sqrt(THREAD_COUNT)){//n is 4 or 16
            int sqr = (int)sqrt(THREAD_COUNT);
            int x = tid % sqr;
            minI = (n/sqr) * x;
            maxI = (n/sqr) * (x+1);

            int y = tid / sqr;
            minJ = (n/sqr) * y;
            maxJ = (n/sqr) * (y+1);
            printf(">>>>>>>>>>>>>>>>>>>>>>%d\n", sqr);
        }else if(THREAD_COUNT == 8){
            int x = tid % 2;
            int y = tid / 2;
            minI = (n/2) * x;
            maxI = (n/2) * (x+1);

            minJ = (n/4) * y;
            maxJ = (n/4) * (y+1);
            
            printf(">>>>>>>>>>>>>>>>>>>>>>%d\n", 8);
        }else if(THREAD_COUNT == 8){
        }
        
        for(int i=minI; i<maxI; i++){
            for(int j=minJ; j<maxJ; j++){
                #pragma omp critical
                histogram[arr[i][j]] ++;
            }
        }

    }
}
int main(){
    int n;
    scanf("%d", &n);

    int** img = (int**)malloc(n* sizeof(int*));
    for (int i = 0; i < n; i++) {
        img[i] = (int*)malloc(n * sizeof(int));
    }

    fill_array(n, img);

    int* histogram = (int*)malloc(sizeof(int)*MAX_NUM);
    for(int i=0; i<n; i++)
        histogram[i] = 0;


    struct timeval start, end; 
    gettimeofday(&start, NULL);
 

    omp_set_num_threads(THREAD_COUNT);
    //parallel_rows(n, img, histogram);
    parallel_columns(n, img, histogram);
    //parallel_blocks(n, img, histogram);

    //for(int i=0; i<n; i++){
      //  for(int j=0; j<n; j++){
        //    printf("%.3d ",img[i][j]);
       // }
        //printf("\n");
   // }

    for(int i=0; i<MAX_NUM; i++){
        if(histogram[i]!=0)
        printf("%d:%d\n",i,histogram[i]);
    }
 
    gettimeofday(&end, NULL);
 
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
    printf("Time elpased %ld microseconds\n", micros);
    
    free(histogram);
    for(int i=0; i<n; i++)
        free(img[i]);
    free(img);
    return 0;
}
