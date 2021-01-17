#include <chrono>
#include <ctime>
#include <fstream>
using namespace std;



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
    

    //DMM on the transposed matrix
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            output[i+j]+=matA[i*N+j]*matB[(N-i-1)*N+j];
        }
    }

    
}
