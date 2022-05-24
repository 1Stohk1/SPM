#include <iostream>
#include <stdlib.h>
#include <vector>

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

void reduce(float *A, int n, vector<float> x, vector<float> &aux)
{
    for (size_t i = 0; i < n; i++) aux[i] = 0;

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            if (i != j) aux[i] += A[i * n + j] * x[j];
        }
    }
}

void Jacobi(float *A, int n, vector<float> &x, vector<float> b, vector<float> aux)
{
    for (size_t i = 0; i < n; i++)
        x[i] =  (b[i] - aux[i])/A[i * n + i];
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

    vector<float> x(n_dim);
    vector<float> b(n_dim);
    float A[n_dim][n_dim];
    vector<float> sol(n_dim);
    vector<float> aux(n_dim);
    for (auto i = 0; i < n_dim; i++)
    {
        x[i] = 0;
        b[i] = 0;
        sol[i] = rand() % MAX;
        for (auto j = 0; j < n_dim; j++)
            A[i][j] = rand() % MAX;
    }
    for (auto i = 0; i < n_dim; i++)
    {
        for (auto j = 0; j < n_dim; j++)
            b[i] += A[i][j] * sol[j];
    }

    for (size_t iter = 0; iter < iterations; iter++)
    {
        reduce(reinterpret_cast<float *>(A), n_dim, x, aux);
        printVec(aux, n_dim, "Printing sum at the iteration " + to_string(iter));
        Jacobi(reinterpret_cast<float *>(A), n_dim, x, b, aux);
        printVec(x, n_dim, "Printing X at the iteration " + to_string(iter));
    }
    printMatrix(reinterpret_cast<float *>(A), n_dim);
    printVec(x, n_dim, "Printing x");
    printVec(b, n_dim, "Printing b");
    printVec(sol, n_dim, "Printing solution"); 
}
