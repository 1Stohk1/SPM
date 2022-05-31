#include <iostream>
#include <stdlib.h>
#include <vector>
#include <functional>
#include "lib/linear_system.hpp"
#include "lib/utimer.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) Check_Results[0/1](bool)" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    bool check = (atoi(argv[3]) == 0 ? false : true);
    srand(123);

    Linear_System ls(n_dim, check);

    function<void()> Jacobi = [&]()
    {
        for (size_t row = 0; row < n_dim; row++)
        {
            ls.x_curr[row] = ls.b[row];
            for (size_t col = 0; col < n_dim; col++)
            {
                if (row != col)
                    ls.x_curr[row] -= ls.A[row][col] * ls.x_old[col];
            }
            ls.x_curr[row] = ls.x_curr[row] / ls.A[row][row];
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
