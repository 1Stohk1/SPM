#include <iostream>
#include <vector>
#include <thread>
#include <barrier>
#include <functional>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace std;

// This file has to be computed with g++(vers.>= 11) Jacobi_std.cpp -pthread -std=c++20 -O3 -o out

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
    
    // Using the library functional to create the updating function to be given to the std::barrier
    function<void()> update = [&](){
        ls.x_old = ls.x_curr;
    };

    //Initialization of the barrier
    barrier sync_point(nw, update);

    /*
    As for the update function here we are creating the core of the algorithm with the barrier at 
    the bottom, this function will be used to start the thread
    */
    function<void(int)> Jacobi = [&](int id)
    {
        for (int t = 0; t < iterations; t++)
        {
            for (int row = id; row < n_dim; row+=nw)
            {
                ls.x_curr[row] = ls.b[row];
                for (size_t col = 0; col < n_dim; col++)
                {
                    if (row != col)
                        ls.x_curr[row] -= ls.A[row][col] * ls.x_old[col];
                }
                ls.x_curr[row] = ls.x_curr[row] / ls.A[row][row];
            }
            sync_point.arrive_and_wait();
        }
        return;
    };

    {
        /*
        Creation of the Timer instance, at the end of this scope the destructor will be called and 
        the time will be printed
        */
        timer_raii tim("Jacobi (std::barrier) " + to_string(n_dim) + " N, " + to_string(iterations) + " Iters, " + to_string(nw) + " Ws");
        vector<thread> tids(nw);

        //Starting...
        for (int i = 0; i < nw; ++i)
        {
            tids[i] = thread(Jacobi, i);
        }
        // ...and joining the threads
        for (auto &t : tids)
        { // await thread termination
            t.join();
        }
    }

    // Checking the results of the system
    ls.print_results();
}