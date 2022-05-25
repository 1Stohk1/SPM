#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <barrier>
#include "utimer.cpp"

using namespace std;

const int MAX = 10;

barrier sync_point(10);

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

void fun(RANGE range, int n_size, vector<float> &x_old, vector<float> &x_curr, vector<float> b, float **A)
{
    for (size_t i = range.start; i < range.end; i++)
    {
        for (size_t j = 0; j < n_size; j++)
        {
            if (i != j)
                x_old[i] += A[i][j] * x_curr[j];
        }
        x_curr[i] = (b[i] - x_old[i]) / A[i][i];
        x_old[i] = 0;
    }
    sync_point.arrive_and_wait();
}

class Jacobi
{
private:
    int num_worker;
    int n_size;
    vector<float> x_old;
    vector<float> x_curr;
    vector<float> sol;
    vector<float> b;
    float **A;

    void sync()
    {
        x_curr = x_old;
    }

public:
    // constructor three
    Jacobi(int n, int nw)
    {
        n_size = n;
        num_worker = nw;

        A = (float **)calloc(n_size, 1 + sizeof *A); // alloc one extra ptr

        for (int i = 0; i < n_size; i++)
        {
            A[i] = (float *)calloc(n_size, sizeof **A);
        }
        A[n] = NULL; // set the extra ptr to NULL
        for (size_t i = 0; i < n; i++)
        {
            x_old.push_back(0);
            x_curr.push_back(0);
            sol.push_back(rand() % MAX); // Creating randomically a solution for the system
            b.push_back(0);
            for (size_t j = 0; j < n; j++)
            {
                if (i == j)
                    A[i][j] = MAX + (rand() % MAX); // Making the diagonally strongly dominant
                else
                    A[i][j] = 1 + (rand() % MAX); // Continuing to fill the matrix randomically
            }
        }
        for (auto i = 0; i < n; i++)
            for (auto j = 0; j < n; j++)
                b[i] += A[i][j] * sol[j]; // Computing the solution vector b of the system
    }

    void map_block()
    {
        int delta{n_size / num_worker};
        vector<RANGE> ranges(num_worker);
        vector<thread> tids(num_worker);

        for (int i = 0; i < num_worker; i++)
        { // split the vector into chunks
            ranges[i].start = i * delta;
            ranges[i].end = (i != (num_worker - 1) ? (i + 1) * delta : n_size - 1);
        }

        for (int i = 0; i < num_worker; ++i)
        {
            tids[i] = (thread(fun, ranges[i], n_size, ref(x_old), ref(x_curr), ref(b), A));
        }

        for (thread &t : tids)
        { // await thread termination
            t.join();
        }
    }

    void print_results()
    {
        printMatrix(A, n_size);
        printVec(x_curr, n_size, "Printing x");
        printVec(b, n_size, "Printing b");
        printVec(sol, n_size, "Printing solution");
    }

    ~Jacobi()
    {
        for (int i = 0; A[i] != NULL; i++)
        {
            free(A[i]);
        }
        free(A);
    }
};

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
    Jacobi jac(n_dim, nw);

    // Solving the system sequentially
    timer_raii tim("Sequentially " + to_string(iterations) + " iterations of the Jacobi have been");
    for (size_t iter = 0; iter < iterations; iter++)
    {
        jac.map_block();
    }
    jac.print_results();
}
