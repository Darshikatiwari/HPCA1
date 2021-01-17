#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;

//Macros for time calculation
#define TIME_DIFF(val, start, stop) std::chrono::duration_cast<val>(stop - start).count()
#define TIME_NOW std::chrono::high_resolution_clock::now()

//transpose of matrix B
void B_transpose(int *dst, const int *src, int n) noexcept 
{
    size_t i,j,k;
    size_t block = 64;

    for (i = 0; i < n; i += block)  //block-wise access
    {
        for(j = 0; j < n; ++j) 
        {
            for(k = 0; k < block && i + k < n; ++k) 
            {
                dst[j*n + k + i] = src[(i + k)*n + j]; //assignment of the transposed values
            }
        }
    }
}


// single threaded function
void singleThread(int N, int *matA, int *matB, int *output)
{

    //pointer to store transposed matrix
    int *dst = new int[N * N];
    
    // transpose calculation
    B_transpose(dst,matB, N);

    //transposed matrix B
    matB=dst;
     
    //time calculation
    begin = TIME_NOW;

    //DMM on the transposed matrix
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            output[i+j]+=matA[i*N+j]*matB[(N-i-1)*N+j];
        }
    }
    
    end = TIME_NOW;
    //Overall program time calculation
    t_time=(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0;
    cout << "\nTime for executing singleThread: " << t_time;    

    
}
