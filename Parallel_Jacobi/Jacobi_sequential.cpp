#include <iostream>
#include <functional>
#include "lib/linear_system.hpp"
#include "lib/utimer.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) "
        "Check_Results[0/1](bool)" << std::endl;
        return (0);
    }
    int n_dim = atoi(argv[1]); // Shape of the square matrix and of the vectors
    int iterations = atoi(argv[2]); // Number of iterations to be computed
    bool check = (atoi(argv[3]) == 0 ? false : true); // Boolean for debugging purposes

    srand(123);

    // Creation of the Linear System instance
    Linear_System ls(n_dim, check);

    /*
    As for the update function here we are creating the core of the algorithm with the barrier at 
    the bottom, this function will be used to start the thread
    */
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

    {
        /*
        Creation of the Timer instance, at the end of this scope the destructor will be called and 
        the time will be printed
        */
        timer_raii tim("Jacobi (sequential) " + to_string(n_dim) + " N, " + to_string(iterations) + " Iters");

        //The Jacobi as in the snippet of pseudo-code is iterate in a loop
        for (size_t iter = 0; iter < iterations; iter++)
        {
            Jacobi(); //The algorithm itself
        }
    }
    
    // Checking the results of the system
    ls.print_results();
}
