#include <iostream>
#include <stdlib.h>
#include <vector>
#include <functional>
#include "lib/linear_system.hpp"
#include "lib/utimer.hpp"

using namespace std;

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

    Linear_System ls(n_dim);

    function<void()> Jacobi = [&]()
    {
        for (size_t row = 0; row < n_dim; row++)
        {
            ls.x_curr[row] = 0;
            for (size_t col = 0; col < n_dim; col++)
            {
                if (row != col)
                    ls.x_curr[row] += ls.A[row][col] * ls.x_old[col];
            }
            ls.x_curr[row] = (ls.b[row] - ls.x_curr[row]) / ls.A[row][row];
        }
        ls.x_old = ls.x_curr;
    };

    { // Solving the system sequentially
        timer_raii tim("Jacobi (sequentially) with " + to_string(iterations) + " iterations");
        for (size_t iter = 0; iter < iterations; iter++)
        {
            Jacobi();
        }
    }
    ls.print_results();
}
