#include <pthread.h>
#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;
 
 // Macros for time calculation
#define TIME_DIFF(val, start, stop) std::chrono::duration_cast<val>(stop - start).count()
#define TIME_NOW std::chrono::high_resolution_clock::now()


//Mutexes for synchronization and mutual exclusion
#define THREAD_COUNT 16                                        //varied depending on the scalibilty of the program


pthread_mutex_t lock_a;
pthread_mutex_t lock_b;
int step =0;
int step1 = 0;

//argument's definition to be passed to the thread
struct thd1 {
        int N;
        int *matA;
        int *matB;
        int *output;
    };

//function executed for DMM
void* mult_threads(void * thread_arg)
{
    //casting from void to struct
    struct thd1 *global = (struct thd1 *) thread_arg;
   
    //synchronization by locks
    pthread_mutex_lock(&lock_a);

    int core = step;      //assignment of cores  
    int start1 =(core*global->N)/THREAD_COUNT;  //range covered by each thread
    int finish1 =((core+1)*global->N)/THREAD_COUNT;  
    step += 1;

    pthread_mutex_unlock(&lock_b);

    //auto begin = TIME_NOW;                                                      (used during profiling of every loop)

    // Iterate over first half of output elements
    for(int i = start1; i < finish1; ++i) {
        int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; ++j) {
            int rowA = j;
            int colA = i - j;
            int rowB = i - j;
            int colB = global->N - j - 1;
            temp += global->matA[rowA * global->N + colA] * global->matB[rowB * global->N + colB];
        }
        global->output[i] = temp;
    }
    //auto end = TIME_NOW;
    //cout << "First Loop " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0; 
    
    //Starting with the second half of the output elements


    //begin = TIME_NOW;(used during profiling of everyloops)

    //synchronization by locks
    pthread_mutex_lock(&lock_a);

    int core1 =step1;
    int start2 =(core1*global->N)/THREAD_COUNT + global->N;
    int finish2=((core1+1)*global->N )/THREAD_COUNT + global->N;
    step1=step1+1;
    
     if (core1== THREAD_COUNT-1)
    {
        finish2 = finish2-1;
    }  
    pthread_mutex_unlock(&lock_b);
   
    // Iterate over second half of output elements
    for(int i = start2; i < finish2; ++i) {
        int temp = 0;
        // Iterate over diagonal elements
        //need to specify N as global
        for(int j = 0; j < 2 * global->N - (i + 1); ++j) {
            int rowA = i + 1 + j - global->N;
            int colA = global->N - j - 1;
            int rowB = global->N - j - 1;
            int colB = 2 * global->N - j - 2 - i;
            temp += global->matA[rowA * global->N + colA] * global->matB[rowB * global->N + colB];
        }
        global->output[i] = temp;
    }

    // end= TIME_NOW;
    //cout << "Second Loop " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 ; 
 

    //thread termination
    pthread_exit(NULL);
}


void multiThread(int N, int *matA, int *matB, int *output)
{
    int Exception_check; //for handling exceptional cases during thread creation
  
    struct thd1 thread[THREAD_COUNT];  //structure array
    pthread_t tid[THREAD_COUNT];     //thread ID for every thread

    //timing counters
    auto begin = TIME_NOW;

    //thread creation and implementation
    for (int i =  0; i < THREAD_COUNT; i++)
    {   
    //argument assignment to function called by every thread
    thread[i].N =N;
    thread[i].matA=matA;
    thread[i].matB=matB;
    thread[i].output=output;   

    //thread creation
    Exception_check = pthread_create(&tid[i], NULL,mult_threads, (void *)&thread[i]);
        
    }
    auto end = TIME_NOW;

    //Exception Check
    if (Exception_check)
    {
        printf("Thread Creation was Unsuccessfull");
    }
    
    //thread synchronization
    int i=0;
    while(i < THREAD_COUNT)
    {
         pthread_join(tid[i],NULL);  
         i++;       
    }
    
    //Time display
    cout << "Total runtime of threads: " << (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 ; 
    


}


