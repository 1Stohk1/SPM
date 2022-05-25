#include <iostream>
#include <cstdlib>
#include <vector>
#include <barrier>
#include <functional>
#include "utimer.cpp"

using namespace std;

//This file has to be computed with g++ std_barrier_parallel.cpp -pthread -std=c++20 -O3 -o out

const int MAX = 10;

typedef struct
{
    int start;
    int end;
} RANGE;

void printMatrix(float **matrix, int n)
{
    cout << "Printing the matrix A" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void printVec(vector<float> vec, int n, string mex)
{
    cout << mex << endl;
    for (int j = 0; j < n; j++)
        cout << vec[j] << " ";
    cout << "\n \n";
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "Usage is: " << argv[0] << " n-dim iterations nw seed" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);
    srand(atoi(argv[4]));
    vector<float> x_old(n_dim);
    vector<float> x_curr(n_dim);
    vector<float> sol(n_dim);
    vector<float> b(n_dim);

    function<void()> x_swap = [&]()
    {
        for (int i = 0; i < n_dim; ++i)
        {
            x_old[i] = x_curr[i];
        }
    };

    barrier sync_point(nw, x_swap);

    float **A;
    A = (float **)calloc(n_dim, 1 + sizeof *A); // alloc one extra ptr

    for (int i = 0; i < n_dim; i++)
    {
        A[i] = (float *)calloc(n_dim, sizeof **A);
    }
    A[n_dim] = NULL; // set the extra ptr to NULL

    for (size_t i = 0; i < n_dim; i++)
    {
        b[i] = 0;
        x_old[i] = 0;
        x_curr[i] = 0;
        sol[i] = rand() % MAX; // Creating randomically a solution for the system
        for (size_t j = 0; j < n_dim; j++)
        {
            if (i == j)
                A[i][j] = MAX + (rand() % MAX); // Making the diagonally strongly dominant
            else
                A[i][j] = 1 + (rand() % MAX); // Continuing to fill the matrix randomically
        }
    }
    for (auto i = 0; i < n_dim; i++)
        for (auto j = 0; j < n_dim; j++)
            b[i] += A[i][j] * sol[j]; // Computing the solution vector b of the system
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;
    {
        start = std::chrono::system_clock::now();
        int delta{n_dim / nw};
        vector<RANGE> ranges(nw);
        vector<thread> tids(nw);

        for (int i = 0; i < nw; i++)
        { // split the vector into chunks
            ranges[i].start = i * delta;
            ranges[i].end = (i != (nw - 1) ? (i + 1) * delta : n_dim);
            cout << "START " << ranges[i].start << " END " << ranges[i].end << " ";
        }
        cout << endl;

        function<void(RANGE)> jacobi = [&](RANGE range)
        {
            float temp;
            for (size_t t = 0; t < iterations; t++)
            {
                for (size_t i = range.start; i < range.end; i++)
                {
                    temp = 0;
                    for (size_t j = 0; j < n_dim; j++)
                    {
                        if (i != j)
                            temp += A[i][j] * x_old[j];
                    }
                    /* cout<<"La sommatoria e' uguale a "<<temp<<endl; */
                    x_curr[i] = (b[i] - temp) / A[i][i];
                    /* cout<<"La variabile calcolata e' "<<x_curr[i]<<endl; */
                }
                sync_point.arrive_and_wait();
            }
            sync_point.arrive_and_drop();
            return;
        };

        for (int i = 0; i < nw; ++i)
        {
            tids[i] = thread(jacobi, ranges[i]);
        }

        for (thread &t : tids)
        { // await thread termination
            t.join();
        }
        stop = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = stop - start;
        auto musec =
            std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Parallel code computed in " << musec << " usec "
                  << std::endl;
    }
    printMatrix(A, n_dim);
    printVec(x_curr, n_dim, "Printing x");
    printVec(b, n_dim, "Printing b");
    printVec(sol, n_dim, "Printing solution");
}