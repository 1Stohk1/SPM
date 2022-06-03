#include <iostream>
#include "fastflow/ff/ff.hpp"
#include "fastflow/ff/parallel_for.hpp"
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) "
        "Number_Workers(int) Check_Results[0/1](bool)" << std::endl;
        return (0);
    }
    int n_dim = atoi(argv[1]); // Shape of the square matrix and of the vectors
    int iterations = atoi(argv[2]); // Number of iterations to be computed
    int nw = atoi(argv[3]); // Max number of workers to use in the parallel computation
    bool check = (atoi(argv[4]) == 0 ? false : true); // Boolean for debugging purposes

    srand(123);

    // Creation of the Linear System instance
    Linear_System ls(n_dim, check);

    {
        /*
        Creation of the Timer instance, at the end of this scope the destructor will be called and 
        the time will be printed
        */
        timer_raii tim("Jacobi (ff::ParallelFor) " + std::to_string(n_dim) + " N, " + std::to_string(iterations)
         + " Iters, " + std::to_string(nw) + " Ws");


        /* 
        Calling the ParallelFor class with spin wait set to false as default and spin barrier set
        to true avoid to use the blocking barrier
        */
        ff::ParallelFor pf(nw, false, true);


        for (int i = 0; i < iterations; i++)
        {
            pf.parallel_for(0, n_dim, 1, n_dim / nw, [&](int id)
                            {
            ls.x_curr[id] = ls.b[id];
            for (size_t col = 0; col < n_dim; col++)
            {
                if (id != col)
                    ls.x_curr[id] -= ls.A[id][col] * ls.x_old[col];
            }
            ls.x_curr[id] = ls.x_curr[id] / ls.A[id][id]; 
            }
            );
            ls.update();
        }

    }

    // Checking the results of the system
    ls.print_results();

    return 0;
}