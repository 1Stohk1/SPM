#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <barrier>
#include <functional>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace std;

// This file has to be computed with g++ std_barrier_parallel.cpp -pthread -std=c++20 -O3 -o out

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) Number_Workers(int) Check_Results[0/1](bool)" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);
    bool check = (atoi(argv[4]) == 0 ? false : true);
    srand(123);

    Linear_System ls(n_dim, check);
    
    function<void()> update = [&](){
        ls.x_old = ls.x_curr;
    };

    barrier sync_point(nw, update);

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
        timer_raii tim("Jacobi (std::barrier) " + to_string(n_dim) + " N, " + to_string(iterations) + " Iters, " + to_string(nw) + " Ws");
        vector<thread> tids(nw);

        for (int i = 0; i < nw; ++i)
        {
            tids[i] = thread(Jacobi, i);
        }

        for (auto &t : tids)
        { // await thread termination
            t.join();
        }
    }
    ls.print_results();
}