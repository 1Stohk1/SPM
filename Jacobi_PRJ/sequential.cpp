#include <iostream>
#include <stdlib.h>
#include <vector>
#include "utimer.cpp"

using namespace std;

const int MAX = 10;

void printMatrix(float *matrix, int n)
{
    cout << "Printing the matrix A" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << matrix[i * n + j] << " ";
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

void reduce(float *A, int n, vector<float> x, vector<float> &x_old)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            if (i != j)
                x_old[i] += A[i * n + j] * x[j];
        }
    }
}

void Jacobi(float *A, int n, vector<float> &x, vector<float> &x_old, vector<float> b)
{
    for (size_t i = 0; i < n; i++)
    {
        x[i] = (b[i] - x_old[i]) / A[i * n + i];
        x_old[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "Usage is: " << argv[0] << " n-dim iterations seed" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    srand(atoi(argv[3]));

    vector<float> x(n_dim);
    vector<float> b(n_dim);
    float A[n_dim][n_dim];
    vector<float> sol(n_dim);
    vector<float> x_old(n_dim);
    for (auto i = 0; i < n_dim; i++)
    {
        sol[i] = rand() % MAX; // Creating the solution of the system randomically
        for (auto j = 0; j < n_dim; j++)
        {
            if (i == j) A[i][j] = MAX + (rand() % MAX); // Making the diagonally strongly dominant
            else A[i][j] = 1 + (rand() % MAX); // Continuing to fill the matrix randomically
        }
    }
    for (auto i = 0; i < n_dim; i++)
        for (auto j = 0; j < n_dim; j++)
            b[i] += A[i][j] * sol[j]; // Computing the solution vector b of the system

    {// Solving the system sequentially
    timer_raii tim("Sequential Jacobi with " + to_string(iterations) + " iterations");
        for (size_t iter = 0; iter < iterations; iter++)
        {
            reduce(reinterpret_cast<float *>(A), n_dim, x, x_old);
            Jacobi(reinterpret_cast<float *>(A), n_dim, x, x_old, b);
        }
    }
    printMatrix(reinterpret_cast<float *>(A), n_dim);
    printVec(x, n_dim, "Printing x");
    printVec(b, n_dim, "Printing b");
    printVec(sol, n_dim, "Printing solution");
}
