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
        {
            cout << matrix[i * n + j] << " ";
        }
        cout <<endl;
    }
cout << "\n \n";

}

void printVec(vector<float> vec, int n, string mex)
{
    cout << mex << endl;
    for (int j = 0; j < n; j++)
    {
        cout << vec[j] << " ";
    }
    cout << "\n \n";
}

void reduce(float *A, int n, vector<float> x, vector<float>& aux)
{
    for (size_t i = 0; i < n; i++)
        aux[i] = 0;

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            if (i != j)
            {
                aux[i] += A[i * n + j] * x[j];
            }
        }
    }
}

void Jacobi(float *A, int n, vector<float>& x, vector<float> b, vector<float> aux)
{
    for (size_t i = 0; i < n; i++)
    {
        x[i] = 1 / A[i * n + i] * (b[i] - aux[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "Usage is: " << argv[0] << " n-dim iterations nw " << endl;
        return (0);
    }
    int n_dim = 3;
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);

    vector<float> x(n_dim);
    vector<float> b(n_dim);
    vector<float> summatory(n_dim);
    float A[n_dim][n_dim];

    b[0] = 1;
    b[1] = 2;
    b[2] = 1;
    A[0][0] = 2; 
    A[0][1] = 6; 
    A[0][2] = 4;
    A[1][0] = 1; 
    A[1][1] = 3; 
    A[1][2] = -1; 
    A[2][0] = -1; 
    A[2][1] = 1; 
    A[2][2] = 2; 

    for (size_t iter = 0; iter < iterations; iter++)
    {
        reduce(reinterpret_cast<float *>(A), n_dim, x, summatory);
        printVec(summatory, n_dim, "Printing sum at the iteration" + to_string(iter));
        Jacobi(reinterpret_cast<float *>(A), n_dim, x, b, summatory);
        printVec(x, n_dim, "Printing X at the iteration" + to_string(iter));
    }
    printMatrix(reinterpret_cast<float *>(A), n_dim);
    printVec(x, n_dim, "Printing x");
    printVec(b, n_dim, "Printing b");
}
