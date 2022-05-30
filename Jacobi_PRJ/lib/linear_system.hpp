#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

const int HI = 20;
const int LO = -9;

const double THRESHOLD = 0.995;

class Linear_System
{
private:
    int n_size;
    bool check;
    void printVec(std::vector<float> vec, int n, std::string mex)
    {
        std::cout << mex << std::endl;
        for (int j = 0; j < n; j++)
            std::cout << vec[j] << " ";
        std::cout << "\n \n";
    }

public:
    float **A;
    std::vector<float> x_old;
    std::vector<float> x_curr;
    std::vector<float> sol;
    std::vector<float> b;
    // constructor three
    Linear_System(int n, bool show)
    {
        n_size = n;
        check = show;
        A = (float **)calloc(n_size, 1 + sizeof *A); // alloc one extra ptr
        for (int i = 0; i < n_size; i++)
            A[i] = (float *)calloc(n_size, sizeof **A);
        A[n_size] = NULL; // set the extra ptr to NULL

        for (size_t i = 0; i < n_size; i++)
        {
            b.push_back(0);
            x_old.push_back(0);
            x_curr.push_back(0);
            sol.push_back(LO + (rand()) % HI); // Creating randomically a solution for the system
            for (size_t j = 0; j < n_size; j++)
            {
                if (i == j)
                    A[i][j] = (LO * n_size) + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO))); // Making the diagonally strongly dominant
                else
                    A[i][j] = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO))); // Continuing to fill the matrix randomically
            }
        }
        for (auto i = 0; i < n_size; i++)
            for (auto j = 0; j < n_size; j++)
                b[i] += A[i][j] * sol[j]; // Computing the solution vector b of the system
    }

    void update()
    {
        x_old = x_curr;
    }

    double cosine_similarity()
    {
        double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
        for (unsigned int i = 0u; i < n_size; ++i)
        {
            dot += sol[i] * x_curr[i];
            denom_a += sol[i] * sol[i];
            denom_b += x_curr[i] * x_curr[i];
        }
        return dot / (sqrt(denom_a) * sqrt(denom_b));
    }

    void print_results()
    {
        if (check)
        {
            std::cout << "MATRIX A" << std::endl;
            for (int i = 0; i < n_size; i++)
            {
                for (int j = 0; j < n_size; j++)
                    printf("%f ", A[i][j]);
                std::cout << std::endl;
            }
            printVec(b, n_size, "VECTOR b");
            printVec(x_curr, n_size, "VECTOR x");
            printVec(sol, n_size, "VECTOR of solution");
        }
        std::cout << (abs(cosine_similarity()) > THRESHOLD ? "Everything fine" : "Something went wrong") << std::endl;
    }

    ~Linear_System()
    {
        for (int i = 0; A[i] != NULL; i++)
        {
            free(A[i]);
        }
        free(A);
    }
};