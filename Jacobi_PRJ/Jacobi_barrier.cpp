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

typedef struct
{
    int start;
    int end;
} RANGE;

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

    Linear_System ls(n_dim);

    function<void()> pt(bind(&Linear_System::update, ls)); //check why it calls the destructor
    
    barrier sync_point(nw, bind(&Linear_System::update, ls));

    function<void(RANGE)> Jacobi = [&](RANGE range)
    {
        for (size_t t = 0; t < iterations; t++)
        {
            for (size_t row = range.start; row < range.end; row++)
            {
                ls.x_curr[row] = 0;
                for (size_t col = 0; col < n_dim; col++)
                {
                    if (row != col)
                        ls.x_curr[row] += ls.A[row][col] * ls.x_old[col];
                }
                ls.x_curr[row] = (ls.b[row] - ls.x_curr[row]) / ls.A[row][row];
            }
            sync_point.arrive_and_wait();
        }
        return;
    };

    {
        timer_raii tim("Jacobi (std::barrier) with " + to_string(iterations) + " iterations");
        int delta{n_dim / nw};
        vector<RANGE> ranges(nw);
        vector<thread> tids(nw);

        for (int i = 0; i < nw; i++)
        { // split the vector into chunks
            ranges[i].start = i * delta;
            ranges[i].end = (i != (nw - 1) ? (i + 1) * delta : n_dim);
        }

        for (int i = 0; i < nw; ++i)
        {
            tids[i] = thread(Jacobi, ranges[i]);
        }

        for (auto &t : tids)
        { // await thread termination
            t.join();
        }
    }
    ls.print_results();
}