#include <iostream>
#include <chrono>
#include <cmath>
using namespace std;

const int size = 100000000;
int A[size], B[size];

template<typename T, typename U>
void copy(T src[], U dst[], int N, int size)
{
    for (int i = size; i >= 0; i = i - N){
        for (int j = i; j > i - N && j >= 0; j--)
            dst[j] = src[j];
    }
}

int main()
{
    for (int i = 0; i < size; i++){
        A[i] = 1997;
        B[i] = -1;
    }

    for (int i = 0; i <= 10; i++){

        int N = pow (2, i);
        // Record start time
        auto start = std::chrono::high_resolution_clock::now();
        
        // copy execute
        copy (A, B, N, size - 1);

        // Record end time
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        std::cout << "Copy executed with N = " << N << " and time taken = " <<  elapsed.count() << " s\n";
    }
}